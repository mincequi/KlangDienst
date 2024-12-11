import 'dart:typed_data';

import 'package:KlangDienst/controllers/filter_controller.dart';
import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/services/eq_service.dart';
import 'package:KlangDienst/usecases/add_filter.dart';
import 'package:get/get.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import '../models/filter_type.dart';

class WebsocketService extends GetxService {
  final EqService _eqService = Get.find<EqService>();

  WebSocketChannel? _channel;

  WebsocketService();

  @override
  void onInit() {
    super.onInit();
    _channel = WebSocketChannel.connect(
      Uri.parse('ws://localhost:8033/ws'),
    );

    // Actually, frontend is the master, but we get initial filters from backend
    _channel!.stream.forEach((element) {
      if (element.runtimeType != Uint8List) return;
      if (element.length % 4 != 0) return;

      List<FilterModel> filters = _convertBufferToFilters(element);
      filters.forEach((filter) {
        AddFilter().call(filter);
      });
      _eqService.updateResponse();

      _subscribeToFilterChanges();
    });
  }

  @override
  void onClose() {
    super.onClose();
    _channel!.sink.close();
  }

  List<FilterModel> _convertBufferToFilters(Uint8List buffer) {
    List<FilterModel> filters = [];
    for (int i = 0; i < buffer.length; i += 4) {
      FilterModel model = FilterModel(
        type: FilterType.values[buffer[i]],
        freqIdx: buffer[i + 1],
        gainIdx: buffer[i + 2] > 127 ? buffer[i + 2] - 256 : buffer[i + 2],
        qIdx: buffer[i + 3],
      );
      filters.add(model);
    }
    return filters;
  }

  bool _subscribed = false;
  void _subscribeToFilterChanges() {
    if (_subscribed) return;
    _eqService.filters.listen((filters) {
      Int8List buffer = Int8List(filters.length * 4);
      for (int i = 0; i < filters.length; i++) {
        FilterModel model = filters[i].model.value;
        buffer[i * 4] = model.type.index;
        buffer[i * 4 + 1] = model.freqIdx;
        buffer[i * 4 + 2] = model.gainIdx;
        buffer[i * 4 + 3] = model.qIdx;
      }
      _channel!.sink.add(buffer);
      _printBuffer(buffer);
    });
    _eqService.filterChanged.listen((filter) {
      if (filter != null) {
        // Find index of filter in list
        int index =
            _eqService.filters.indexWhere((element) => element == filter);
        FilterModel model = filter.model.value;
        Int8List buffer = Int8List(5);
        buffer[0] = index;
        buffer[1] = model.type.index;
        buffer[2] = model.freqIdx;
        buffer[3] = model.gainIdx;
        buffer[4] = model.qIdx;
        _channel!.sink.add(buffer);
        _printBuffer(buffer);
      }
    });
    _subscribed = true;
  }

  void _printBuffer(Int8List buffer) {
    Int16List p = Int16List(buffer.length);
    for (int i = 0; i < buffer.length; i++) {
      p[i] = buffer[i];
    }
    print(p);
  }
}
