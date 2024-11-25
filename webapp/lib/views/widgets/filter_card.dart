import 'package:flutter/material.dart';
import 'package:get/get.dart';
import '/controllers/filter_controller.dart';

// The Card that displays each filter
class FilterCard extends StatelessWidget {
  final FilterController controller;

  FilterCard({required this.controller});

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: EdgeInsets.all(8),
      elevation: 4,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            // Filter Type Dropdown
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text('Filter Type:', style: TextStyle(fontSize: 16)),
                Obx(() => DropdownButton<String>(
                      value: controller.selectedFilter.value,
                      items: [
                        'Low Pass',
                        'High Pass',
                        'Band Pass',
                        'Notch',
                      ].map((filter) {
                        return DropdownMenuItem<String>(
                          value: filter,
                          child: Text(filter),
                        );
                      }).toList(),
                      onChanged: (value) {
                        if (value != null) {
                          controller.setFilterType(value);
                        }
                      },
                    )),
              ],
            ),
            SizedBox(height: 16),
            // Frequency Setting
            _buildSettingRow(
              'Frequency (Hz)',
              controller.frequency,
              controller.incrementFrequency,
              controller.decrementFrequency,
            ),
            SizedBox(height: 16),
            // Gain Setting
            _buildSettingRow(
              'Gain (dB)',
              controller.gain,
              controller.incrementGain,
              controller.decrementGain,
            ),
            SizedBox(height: 16),
            // Q Factor Setting
            _buildSettingRow(
              'Q Factor',
              controller.qFactor,
              controller.incrementQFactor,
              controller.decrementQFactor,
            ),
          ],
        ),
      ),
    );
  }

  // Helper Widget to Build Setting Rows
  Widget _buildSettingRow(
    String label,
    Rx<dynamic> value,
    VoidCallback onIncrement,
    VoidCallback onDecrement,
  ) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Text(label, style: TextStyle(fontSize: 16)),
        Row(
          children: [
            IconButton(
              onPressed: onDecrement,
              icon: Icon(Icons.remove),
            ),
            Obx(() => Text(
                  value.value.toStringAsFixed(1),
                  style: TextStyle(fontSize: 16),
                )),
            IconButton(
              onPressed: onIncrement,
              icon: Icon(Icons.add),
            ),
          ],
        ),
      ],
    );
  }
}
