test:
	 g++ lr_unit_test.cpp ./../../linear_regression/linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_unit_test
	 ./lr_unit_test
test-debug:
	 g++  -g -DDEBUG lr_unit_test.cpp ./../../linear_regression/linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_unit_test
web-run:
	 g++ -DWEB_RUN lr_unit_test.cpp ./../../linear_regression/linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_unit_test_web_run
	 ./lr_unit_test_web_run
web-run-debug:
	 g++  -g -DWEB_RUN -DDEBUG lr_unit_test.cpp ./../../linear_regression/linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_unit_test_web_run
clean:
	 rm ./lr_unit_test
	 rm ./lr_unit_test_web_run

