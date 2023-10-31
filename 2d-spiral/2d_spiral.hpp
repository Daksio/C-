int** MakeSpiral(int n) {
  int** spiral = new int*[n];
  for (int i = 0; i < n; ++i) {
    spiral[i] = new int[n];
  }
  int number = 1;
  if (n % 2 == 1) {
    spiral[n / 2][n / 2] = n * n;
  }
  for (int circle = 0; circle < n / 2; ++circle) {
    for (int i = circle; i < n - circle; ++i) {
      spiral[circle][i] = number++;
    }
    for (int i = circle + 1; i < n - circle; ++i) {
      spiral[i][n - circle - 1] = number++;
    }
    for (int i = n - circle - 2; i >= circle; --i) {
      spiral[n - circle - 1][i] = number++;
    }
    for (int i = n - circle - 2; i > circle; --i) {
      spiral[i][circle] = number++;
    }
  }
  return spiral;
}
