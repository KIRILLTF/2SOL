#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

class ArrayGenerator {
 public:
  ArrayGenerator(int maxSize, int range)
      : maxSize(maxSize), range(range), engine(std::random_device{}()) {}

  std::vector<int> generateRandom() {
    std::vector<int> arr(maxSize);
    std::uniform_int_distribution<int> dist(0, range);
    for (int& x : arr) x = dist(engine);
    return arr;
  }

  std::vector<int> generateReversed() {
    std::vector<int> arr(maxSize);
    std::iota(arr.rbegin(), arr.rend(), 1);
    return arr;
  }

  std::vector<int> generateNearlySorted(int swaps) {
    std::vector<int> arr(maxSize);
    std::iota(arr.begin(), arr.end(), 1);
    std::uniform_int_distribution<int> dist(0, maxSize - 1);
    for (int i = 0; i < swaps; ++i) {
      int idx1 = dist(engine), idx2 = dist(engine);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }

 private:
  int maxSize;
  int range;
  std::mt19937 engine;
};

class SortTester {
 public:
  static long long timeMergeSort(std::vector<int> arr) {
    std::vector<int> temp(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1, temp);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
        .count();
  }

  static long long timeHybridSort(std::vector<int> arr) {
    std::vector<int> temp(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    hybridMergeSort(arr, 0, arr.size() - 1, temp);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
        .count();
  }

 private:
  static void mergeSort(std::vector<int>& arr, int left, int right,
                        std::vector<int>& temp) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, temp);
    mergeSort(arr, mid + 1, right, temp);
    merge(arr, left, mid, right, temp);
  }

  static void merge(std::vector<int>& arr, int left, int mid, int right,
                    std::vector<int>& temp) {
    int i = left, j = mid + 1, k = left;
    for (int idx = left; idx <= right; ++idx) temp[idx] = arr[idx];
    while (i <= mid && j <= right) {
      if (temp[i] <= temp[j])
        arr[k++] = temp[i++];
      else
        arr[k++] = temp[j++];
    }
    while (i <= mid) arr[k++] = temp[i++];
    while (j <= right) arr[k++] = temp[j++];
  }

  static void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
      int key = arr[i], j = i - 1;
      while (j >= left && arr[j] > key) {
        arr[j + 1] = arr[j];
        --j;
      }
      arr[j + 1] = key;
    }
  }

  static void hybridMergeSort(std::vector<int>& arr, int left, int right,
                              std::vector<int>& temp) {
    const int threshold = 15;
    if (right - left + 1 <= threshold) {
      insertionSort(arr, left, right);
      return;
    }
    int mid = left + (right - left) / 2;
    hybridMergeSort(arr, left, mid, temp);
    hybridMergeSort(arr, mid + 1, right, temp);
    if (arr[mid] <= arr[mid + 1]) return;
    merge(arr, left, mid, right, temp);
  }
};

int main() {
  int maxSize = 10000, range = 6000, swaps = 10;
  ArrayGenerator generator(maxSize, range);

  std::vector<int> randomArray = generator.generateRandom();
  std::vector<int> reversedArray = generator.generateReversed();
  std::vector<int> nearlySortedArray = generator.generateNearlySorted(swaps);

  for (int size = 500; size <= maxSize; size += 100) {
    std::vector<int> randomSubArray(randomArray.begin(),
                                    randomArray.begin() + size);
    std::vector<int> reversedSubArray(reversedArray.begin(),
                                      reversedArray.begin() + size);
    std::vector<int> nearlySortedSubArray(nearlySortedArray.begin(),
                                          nearlySortedArray.begin() + size);

    long long mergeTime = SortTester::timeMergeSort(randomSubArray);
    long long hybridTime = SortTester::timeHybridSort(randomSubArray);

    std::cout << "Size: " << size << ", MergeSort: " << mergeTime
              << "ms, HybridSort: " << hybridTime << "ms\n";
  }

  return 0;
}
