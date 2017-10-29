//
//  Created by Boyd Multerer on 10/26/17.
//  Copyright © 2017 Kry10 Industries. All rights reserved.
//

//#include <stdio.h>

#include <stdbool.h>
//#include <string.h>
#include <math.h>
#include <erl_nif.h>

#include "erl_utils.h"


//=============================================================================
// line math




//=============================================================================
// Erlang NIF stuff from here down.

//-----------------------------------------------------------------------------
// find a parallel line to the given one, at a specified distance away
static ERL_NIF_TERM
nif_parallel(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  double x0, y0, x1, y1;
  double w;
  double x, y, d, t;

  // get the parameters
  if ( !get_double_num(env, argv[0], &x0) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[1], &y0) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[2], &x1) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[3], &y1) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[4], &w) )      {return enif_make_badarg(env);}

  x = x0 - x1;
  y = y0 - y1;

  d = sqrt( (x * x) + (y * y) );

  x = x / d;
  y = y / d;

  t = x;
  x = -y;
  y = t;

  x0 = x0 + (w * x);
  y0 = y0 + (w * y);
  x1 = x1 + (w * x);
  y1 = y1 + (w * y);

  return enif_make_tuple2( env,
    enif_make_tuple2( env,
      enif_make_double(env, x0),
      enif_make_double(env, y0)
    ), enif_make_tuple2( env,
      enif_make_double(env, x1),
      enif_make_double(env, y1)
    )
  );
}
/*
#  def parallel({{x0, y0}, {x1, y1}} , w ) do
#    {x,y} = Math.Vector2.sub({x0, y0}, {x1, y1})
#    div = :math.sqrt(Math.Vector2.dot({x,y}, {x,y}))
#    {x,y} = Math.Vector2.div({x,y}, div)
#    {x,y} = {-y,x}
#    {{x0 + w*x, y0 + w*y}, {x1 + w*x, y1 + w*y}}
#  end
*/


//-----------------------------------------------------------------------------
// find a point that is the intersection of two lines
static ERL_NIF_TERM
nif_intersection(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    double x0, y0, x1, y1, x2, y2, x3, y3;
    double x, y;
    double d, d0, d1;

  // get the parameters
  if ( !get_double_num(env, argv[0], &x0) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[1], &y0) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[2], &x1) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[3], &y1) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[4], &x2) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[5], &y2) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[6], &x3) )     {return enif_make_badarg(env);}
  if ( !get_double_num(env, argv[7], &y3) )     {return enif_make_badarg(env);}

  d = (x0-x1) * (y2-y3) - (y0-y1) * (x2-x3);
  d0 = x0*y1 - y0*x1;
  d1 = x2*y3 - y2*x3;
  x = (d0*(x2-x3) - d1*(x0-x1)) / d;
  y = (d0*(y2-y3) - d1*(y0-y1)) / d;

  return enif_make_tuple2( env,
      enif_make_double(env, x),
      enif_make_double(env, y)
    );
}
/*
#  def intersection({{x0, y0}, {x1, y1}}, {{x2, y2}, {x3, y3}}) do
#    d = (x0-x1)*(y2-y3)-(y0-y1)*(x2-x3)
#    d0 = x0*y1-y0*x1
#    d1 = x2*y3-y2*x3
#    x = d0*(x2-x3)-d1*(x0-x1)
#    y = d0*(y2-y3)-d1*(y0-y1)
#    {x / d, y / d}
#  end
*/

//=============================================================================
// erl housekeeping. This is the list of functions available to the erl side

static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
//  {"do_put", 4, nif_put},
  {"nif_parallel",          5, nif_parallel},
  {"nif_intersection",      8, nif_intersection}
};


ERL_NIF_INIT(Elixir.Scenic.Math.Line, nif_funcs, NULL, NULL, NULL, NULL)


