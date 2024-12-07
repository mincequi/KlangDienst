import 'package:KlangDienst/models/filter.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:get/get_connect/http/src/utils/utils.dart';
import '/controllers/filter_controller.dart';
import 'filter_param.dart';

// The Card that displays each filter
class FilterCard extends StatelessWidget {
  final FilterController filterController;

  FilterCard({required this.filterController});

  @override
  Widget build(BuildContext context) {
    return Obx(() => Card(
          shape: FilterController.activeController.value == filterController
              ? RoundedRectangleBorder(
                  side: BorderSide(
                    color:
                        Theme.of(context).colorScheme.primary, // Border color
                    width: 1.0, // Border width
                  ),
                  borderRadius: BorderRadius.circular(8.0), // Rounded corners
                )
              : null,
          child: InkWell(
              onTap: () {
                FilterController.activeController.value = filterController;
              },
              child: Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: Column(children: [
                    Row(
                      //mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Obx(() => DropdownMenu<FilterType>(
                              initialSelection: filterController.type.value,
                              dropdownMenuEntries: [
                                FilterType.Bypass,
                                FilterType.Peaking,
                                FilterType.LowPass,
                                FilterType.HighPass,
                                FilterType.LowShelf,
                                FilterType.HighShelf,
                              ].map((filter) {
                                return DropdownMenuEntry<FilterType>(
                                  value: filter,
                                  label: filter.toString().split('.').last,
                                  //child: Text(filter),
                                );
                              }).toList(),
                              onSelected: (value) {
                                if (value != null) {
                                  filterController.setFilterType(value);
                                }
                              },
                            )),
                      ],
                    ),
                    FilterParam(
                        "F",
                        filterController.freqUnit,
                        filterController.freq,
                        filterController.incFreq,
                        filterController.decFreq),
                    FilterParam(
                        "G",
                        filterController.gainUnit,
                        filterController.gain,
                        filterController.incGain,
                        filterController.decGain),
                    FilterParam("Q", filterController.qUnit, filterController.q,
                        filterController.incQ, filterController.decQ),
                  ]))),
        ));
  }
}
