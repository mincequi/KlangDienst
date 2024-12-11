import 'package:KlangDienst/models/filter_type.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';
import '../../services/eq_service.dart';
import '/controllers/filter_controller.dart';
import 'filter_param.dart';

// The Card that displays each filter
class FilterCard extends StatelessWidget {
  final FilterController _controller;
  final _eqService = Get.find<EqService>();

  FilterCard({required FilterController filterController})
      : _controller = filterController;

  @override
  Widget build(BuildContext context) {
    return Obx(
      () => Card(
          shape: _controller.isActive()
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
                FilterController.activeController.value = _controller;
              },
              child: /*Padding(
                  padding: const EdgeInsets.all(8.0),
                  child:*/
                  Column(children: [
                Row(
                  //mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Expanded(
                        child: Obx(() => DropdownMenu<FilterType>(
                              initialSelection: _controller.type.value,
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
                                  _controller.setFilterType(value);
                                }
                              },
                              requestFocusOnTap: false,
                              expandedInsets: EdgeInsets.zero,
                            ))),
                    //SizedBox(width: 36),
                    Column(
                        //alignment: Alignment.bottomRight,
                        children: [
                          IconButton(
                            visualDensity: VisualDensity.compact,
                            icon: Icon(
                              Icons.delete_forever,
                              color: Theme.of(context)
                                  .colorScheme
                                  .error, // Icon color
                            ),
                            onPressed: () {
                              _eqService.removeFilter(_controller);
                            },
                          ),
                          SizedBox(height: 10),
                        ]),
                  ],
                ),
                FilterParam("F", _controller.freqEnabled, _controller.freqUnit,
                    _controller.freq, _controller.incFreq, _controller.decFreq),
                FilterParam("G", _controller.gainEnabled, _controller.gainUnit,
                    _controller.gain, _controller.incGain, _controller.decGain),
                FilterParam("Q", _controller.qEnabled, _controller.qUnit,
                    _controller.qReadout, _controller.incQ, _controller.decQ),
              ]))),
    );
  }
}
