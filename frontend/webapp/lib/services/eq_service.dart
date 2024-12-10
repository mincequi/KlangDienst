import 'dart:math';

import 'package:KlangDienst/models/filter_model.dart';
import 'package:KlangDienst/utils/freq_table.dart';
import 'package:get/get.dart';
import '../controllers/filter_controller.dart';

class EqService extends GetxService {
  final filters = <FilterController>[].obs;

  final filterChanged = Rx<FilterController?>(null);
  final response = List.filled(freqs.length, 0.0).obs;

  EqService();

  void addFilter() {
    FilterController controller = FilterController();
    filters.add(controller);

    // Subscribe to changes in filter parameters and bind filter's index
    ever(controller.model, (_) {
      filterChanged.value = controller;
      filterChanged.refresh();
    });

    // Subscribe to changes in filter responses
    ever(filters.last.response, (_) {
      _updateResponse();
    });
  }

  void removeFilter(FilterController controller) {
    filters.remove(controller);
    _updateResponse();
  }

  void _onFilterChanged() {}

  void _updateResponse() {
    List<double> combinedResponse = List.filled(freqs.length, 0.0);
    for (var filter in filters) {
      for (int i = 0; i < filter.response.length; i++) {
        combinedResponse[i] += filter.response[i];
      }
    }
    response.assignAll(combinedResponse);
    response.refresh();
  }
}
