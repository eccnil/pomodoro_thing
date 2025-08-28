info: 
	@echo "info       -> show (main) targets and a description"
	@echo "unit_test  -> pruebas unitarias en desktop"
	@echo "fuses      -> burn fuses on chip"
	@echo "run        -> pio run"
	
unit_test: test/test_pomodoro/a.out
	./test/test_pomodoro/a.out

test/test_pomodoro/a.out: test/test_pomodoro/test_main.cpp src/pomodoro.cpp
	g++ -o test/test_pomodoro/a.out test/test_pomodoro/test_main.cpp

fuses:
	pio run --target fuses

run:
	pio run

