import 'package:KlangDienst/utils/freq_table.dart';
import 'package:get/get.dart';
import '../controllers/filter_controller.dart';

class EqService extends GetxService {
  final filters = <FilterController>[].obs;
  var filterCount = 0.obs;

  final response = List.filled(freqs.length, 0.0).obs;

  EqService() {
    //addFilter();
  }

  void addFilter() {
    filterCount++;
    filters.add(FilterController());

    // Subscribe to changes in filter responses
    ever(filters.last.response, (_) {
      List<double> combinedResponse = List.filled(freqs.length, 0.0);
      for (var filter in filters) {
        for (int i = 0; i < filter.response.length; i++) {
          combinedResponse[i] += filter.response[i];
        }
      }
      response.assignAll(combinedResponse);
      response.refresh();
    });
  }

  void removeFilter(FilterController controller) {
    filterCount--;
    filters.remove(controller);
  }
}
