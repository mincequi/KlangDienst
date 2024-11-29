import 'package:get/get.dart';
import 'filter_controller.dart';

class FilterManagerController extends GetxController {
  var filters = <FilterController>[].obs;
  var filterCount = 0.obs;

  FilterManagerController() {
    addFilter();
  }

  void addFilter() {
    filterCount++;
    filters.add(FilterController());
  }

  void removeFilter(FilterController controller) {
    filterCount--;
    filters.remove(controller);
  }
}
