test:
	docker build -t cs4500:0.1 .
	cp ./src/*.h ./part1
	- docker run -ti -v "$$(pwd)/src":/src cs4500:0.1 bash -c "cd ./src; cmake ."
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make personal_test_suite && valgrind --leak-check=yes ./personal_test_suite"
