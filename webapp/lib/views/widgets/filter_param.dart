import 'package:KlangDienst/views/widgets/hold_icon_button.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class FilterParam extends StatelessWidget {
  FilterParam(this.label, this.isEnabled, this.unit, this.value,
      this.onIncrement, this.onDecrement);

  String label;
  RxBool isEnabled = false.obs;
  RxString unit;
  RxString value;
  VoidCallback onIncrement;
  VoidCallback onDecrement;

  @override
  Widget build(BuildContext context) {
    return Padding(
        padding: EdgeInsets.only(left: 16),
        child: Obx(() => Opacity(
            opacity: isEnabled.value ? 1.0 : 0.5,
            child: IgnorePointer(
                ignoring: !isEnabled.value,
                child: Row(
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
                    HoldIconButton(
                        icon: Icon(Icons.remove), onPressed: onDecrement),
                    HoldIconButton(
                        icon: Icon(Icons.add), onPressed: onIncrement)
                  ],
                )))));
  }
}
