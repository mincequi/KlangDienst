import 'package:KlangDienst/views/widgets/hold_icon_button.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class FilterParam extends StatelessWidget {
  FilterParam(
      this.label, this.unit, this.value, this.onIncrement, this.onDecrement);

  String label;
  RxString unit;
  RxString value;
  VoidCallback onIncrement;
  VoidCallback onDecrement;

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Text(label),
        SizedBox(width: 8),
        const Spacer(),
        // value to string with three significant digits
        Obx(() => Text(value.value)),
        SizedBox(width: 4),
        Obx(() => Text(unit.value)),
        //Text(value.value.toString()),
        SizedBox(width: 8),
        HoldIconButton(icon: Icon(Icons.remove), onPressed: onDecrement),
        HoldIconButton(icon: Icon(Icons.add), onPressed: onIncrement)
      ],
    );
  }
}
