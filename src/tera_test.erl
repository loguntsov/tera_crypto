-module(tera_test).

-compile(export_all).

key() ->
  list_to_binary(lists:flatten([ [ 50, 32, 78, 2, 9 ] || _ <- lists:seq(1, 34) ])).
test() ->
  Key = key(),
  { ok, Protocol } = tera_crypto:new(Key),
  tera_crypto:encode(Protocol, <<"HELLLLLO_HELLO_HHHHHELLO">>).
