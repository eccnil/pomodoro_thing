info: 
	@echo "info       -> show (main) targets and a description"
	@echo "unit_test  -> pruebas unitarias en desktop"
	@echo "fuses      -> burn fuses on chip"
	@echo "run        -> pio run"
	
unit_test: test/test_pomodoro/a.out
	./test/test_pomodoro/a.out

test/test_pomodoro/a.out: test/test_pomodoro/test_main.cpp src/pomodoro.cpp include/pomodoro.hpp
	g++ test/test_pomodoro/test_main.cpp -I include -o test/test_pomodoro/a.out 

fuses:
	pio run --target fuses

run:
	pio run

