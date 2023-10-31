#pragma once

int* LowerBound(int* first, int* last, int value) {
  if (*first >= value) {
    return first;
  }
  while (last - first > 1) {
    if (*(first + (last - first) / 2) < value) {
      first = first + (last - first) / 2;
    } else {
      last = first + (last - first) / 2;
    }
  }
  return last;
}