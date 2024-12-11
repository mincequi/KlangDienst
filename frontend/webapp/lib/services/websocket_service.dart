import 'dart:typed_data';

import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/services/eq_service.dart';
import 'package:get/get.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import '../models/filter_type.dart';

class WebsocketService extends GetxService {
  EqService _eqService = Get.find<EqService>();

  WebSocketChannel? _channel;

  WebsocketService();

  @override
  void onInit() {
    super.onInit();
    _channel = WebSocketChannel.connect(
      Uri.parse('ws://localhost:8033/ws'),
    );
    _channel!.stream.forEach((element) {
      if (element.runtimeType != Int8List) return;
      if (element.length % 4 != 0) return;

      // Convert Int8List to List<FilterModel>
      List<FilterModel> filters = [];
      for (int i = 0; i < element.length; i += 4) {
        FilterModel model = FilterModel(
          type: FilterType.values[element[i]],
          freqIdx: element[i + 1],
          gainIdx: element[i + 2],
          qIdx: element[i + 3],
        );
        filters.add(model);
      }
      //_eqService.filters.value = filters;
    });
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
      printBuffer(buffer);
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
        printBuffer(buffer);
      }
    });
  }

  @override
  void onClose() {
    super.onClose();
    _channel!.sink.close();
  }

  void printBuffer(Int8List buffer) {
    Int16List p = Int16List(buffer.length);
    for (int i = 0; i < buffer.length; i++) {
      p[i] = buffer[i];
    }
    print(p);
  }
}
