test:
	 g++ ./driver.cpp ./../linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_run
test-debug:
	 g++ -g -DDEBUG ./driver.cpp ./../linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_run
web-run:
	 g++ -DWEB_RUN ./driver.cpp ./../linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_run_web
web-run-debug:
	 g++ -g -DWEB_RUN -DDEBUG ./driver.cpp ./../linear_regression_nd/lr.cpp ./../../utils/csv_helper.cpp -o lr_run_web
clean:
	 rm ./lr_run
	 rm ./lr_run_web
