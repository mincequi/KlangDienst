import 'package:KlangDienst/models/filter_model.dart';
import 'package:get/get.dart';

import '../controllers/filter_controller.dart';
import '../services/eq_service.dart';

class AddFilter {
  final EqService _eqService = Get.find<EqService>();

  void call(FilterModel model) {
    FilterController controller = FilterController.fromModel(model);
    _eqService.filters.add(controller);

    // Subscribe to changes in filter parameters and bind filter's index
    ever(controller.model, (_) {
      _eqService.filterChanged.value = controller;
      _eqService.filterChanged.refresh();
    });

    // Subscribe to changes in filter responses
    ever(controller.response, (_) {
      _eqService.updateResponse();
    });
  }
}
