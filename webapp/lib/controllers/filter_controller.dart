import 'package:get/get.dart';

// Controller to manage the states
// FilterController for individual filter settings
class FilterController extends GetxController {
  var selectedFilter = 'Low Pass'.obs;
  var frequency = 100.0.obs;
  var gain = 1.0.obs;
  var qFactor = 0.7.obs;

  // Increment and Decrement methods
  void incrementFrequency() => frequency.value += 10;
  void decrementFrequency() => frequency.value -= 10;

  void incrementGain() => gain.value += 1;
  void decrementGain() => gain.value -= 1;

  void incrementQFactor() => qFactor.value += 0.1;
  void decrementQFactor() => qFactor.value -= 0.1;

  void setFilterType(String type) {
    selectedFilter.value = type;
  }
}
