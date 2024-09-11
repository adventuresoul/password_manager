all: final

final: password_manager.o
		g++ password_manager.o -o app
		@echo "app executable created"

password_manager.o: password_manager.cpp
		g++ -c password_manager.cpp

clean:
		rm -f password_manager.o app
