import 'package:flutter/material.dart';
import 'package:get/get.dart';
import '/controllers/filter_controller.dart';
import 'filter_param.dart';

// The Card that displays each filter
class FilterCard extends StatelessWidget {
  final FilterController filterController;

  FilterCard({required this.filterController});

  @override
  Widget build(BuildContext context) {
    return Card(
      //color: Colors.blueAccent,
      child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Column(children: [
            Row(
              //mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Obx(() => DropdownMenu<String>(
                      initialSelection: filterController.type.value,
                      dropdownMenuEntries: [
                        'Peaking',
                        'LowPass',
                        'HighPass',
                        'LowShelf',
                        'HighShelf',
                      ].map((filter) {
                        return DropdownMenuEntry<String>(
                          value: filter,
                          label:
                              filter, // Optional, adds accessibility/description.
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
            FilterParam("F", filterController.freqUnit, filterController.freq,
                filterController.incFreq, filterController.decFreq),
            FilterParam("G", filterController.gainUnit, filterController.gain,
                filterController.incGain, filterController.decGain),
            FilterParam("Q", filterController.qUnit, filterController.q,
                filterController.incQ, filterController.decQ),
          ])),
    );
  }
}
