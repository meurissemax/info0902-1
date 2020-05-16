errMin(n, k, error, intervals, histogram, nLevels) {
	err = 0
	minErr = +INFINITY
	index = getIndex(n, k, nLevels)

	if(k == n)
		error[index] = 0;
		return 0;

	if(k == 0)
		error[index] = computeError(histogram, 0, n);
		intervals[index] = n;
		return error[index];

	if(error[index] != +INFINITY)
		return error[index];

	for(i = n - 1; i > k - 1; i--)
		err = computeError(histogram, i, n) + errMin(i, k - 1, error, intervals, histogram, nLevels);

		if(err < minErr)
			minErr = err;
			intervals[index] = n - i + 1;

	error[index] = minErr;

	return minErr;
}