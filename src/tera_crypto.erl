-module(tera_crypto).

-export([
    new/1,
    encode/2
]).

-on_load(init/0).

init() ->
    SoName = case code:priv_dir(tera_protocol) of
        {error, bad_name} ->
            case code:which(?MODULE) of
                Filename when is_list(Filename) ->
                    filename:join([filename:dirname(Filename),"../priv", "tera_crypto"]);
                _ ->
                    filename:join("../priv", "tera_crypto")
            end;
        Dir ->
            filename:join(Dir, "tera_crypto")
    end,
    erlang:load_nif(SoName, 0).

-spec new(Key :: binary()) -> { ok, Object :: reference() }.
new(_Key) ->
    erlang:nif_error({error, not_loaded}).

-spec encode(Object :: reference(), Buffer :: binary()) -> NewBuffer :: binary().
encode(_Ref, _Binary) ->
    erlang:nif_error({error, not_loaded}).