import 'package:KlangDienst/utils/freq_table.dart';
import 'package:get/get.dart';
import '../controllers/filter_controller.dart';

class EqService extends GetxService {
  EqService();

  final filters = <FilterController>[].obs;
  final filterChanged = Rx<FilterController?>(null);
  final response = List.filled(freqs.length, 0.0).obs;

  void removeFilter(FilterController controller) {
    filters.value = filters.where((f) => f != controller).toList();
    //filters.remove(controller);
    updateResponse();
  }

  void _onFilterChanged() {}

  void updateResponse() {
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
