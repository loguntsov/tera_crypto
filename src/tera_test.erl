-module(tera_test).

-compile(export_all).

key() ->
  list_to_binary([ 0 || _ <- lists:seq(1, 170) ]).
test() ->
  Key = key(),
  { ok, Protocol } = tera_crypto:new(Key),
  tera_crypto:encode(Protocol, <<"HELLO">>).
