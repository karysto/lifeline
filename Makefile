build:
	pebble build

install:
	pebble intstall --logs

deploy:
	pebble build && pebble install --logs

.PHONY: build install deploy
