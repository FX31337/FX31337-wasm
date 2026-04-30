# Copilot Instructions

## Indicator testing flow

Starting point for testing indicator is the `/tests/js/IndicatorTest.js` file. It loads `/dist/IndicatorTest.wasm`
that was compiled from the `/src/IndicatorTest.cpp` file.

`IndicatorTest.cpp` file exposes the `Tester` class used to run the test.

In order to run test via `Tester` class, we need to:

- Initialize a `IndicatorTick`-derived class that will provide ticks for all indicators in the test,
  e.g. `TickProvider` from the `indicators` namespace in the library object.
- Initialize a `IndicatorTf`-derived class that will provide timeframe to the indicator(s) we want to test,
  e.g. `Tf` from the `indicators` namespace in the library object.
- Set the source of the `IndicatorTf`-derived class to the `IndicatorTick`-derived class, e.g. `tfM5.SetSource(ticker)`.
- Initialize the indicator(s) we want to test, e.g. `RSI` from the `indicators` namespace in the library object.
- Set the source of the indicator(s) to the `IndicatorTf`-derived class, e.g. `rsiM5.SetSource(tfM5)`.
- Add the indicator(s) to the tester, e.g. `tester.Add(rsiM5)`
- Feed the `IndicatorTick`-derived class with ticks, e.g. `ticker.Feed(ticks)`, where `ticks` is a `TickTABArray`
  object or via `tester.FeedTickProvider(ticker)` if we want to feed tick provider with random ticks.
- Run the test, e.g. `tester.Run()`
- Check the results, e.g. `tester.GetResults()`.

A simple example of the test can be found in the `/tests/js/IndicatorTest.ts` file.

## Notes

### What does TimeCurrent() return

In `IndicatorData::Tick()` we update the time of the current tick (which can be retrieved using `TimeCurrent()`)
by calling `OnUpdatePlatformTime()` before calling `OnTick()` for the indicator.

Indicator could then use `TimeCurrent()` to get the time of the current tick,
e.g. to check if new tick belongs to new bar or not.
