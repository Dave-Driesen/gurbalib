inherit M_MESSAGES;

mapping permanent;
mapping colors;
mapping imud;
mapping guilds;
static mapping channels;
static mapping listeners;

#include <channel.h>

void restore_me( void );
void save_me( void );
void chan_new( string name, int flags );
void chan_join( string chan, object ob );
void chan_leave( string chan, object ob );
void chan_say( string chan, string what );
void chan_emote( string chan, string what );
void chan_who( string chan );
int query_subscribed( string chan );
void chan_make_permanent( string name );
void chan_set_color( string chan, string col );
void chan_set_guild( string chan, string guild );
void chan_set_flag( string chan, int flag );
void chan_imud( string chan, string name );

void create( void ) {
  string *chans;
  int i;

  permanent = ([ ]);
  channels = ([ ]);
  listeners = ([ ]);
  colors = ([ ]);
  imud = ([ ]);
  guilds = ([ ]);
  restore_me();
  chans = map_indices( permanent );
  for( i = 0; i < sizeof( chans ); i++ ) {
    channels[chans[i]] = permanent[chans[i]];
  }

  EVENT_D->subscribe_event( "player_login" );
  EVENT_D->subscribe_event( "player_logout" );
  EVENT_D->subscribe_event( "new_player" );
}
void chan_cmd( string chan, string cmd ) {

  switch( cmd ) {
  case "/on":
  case "/join":
    chan_join( chan, this_player() );
    this_player()->add_channel( chan );
    break;
  case "/off":
  case "/leave":
  case "/quit":
    chan_leave( chan, this_player() );
    this_player()->remove_channel( chan );
    write( "Not subscribed to " + chan + ".\n" );
    break;
  case "/new":
    chan_new( chan, ALL );
    break;
  case "/admin":
    chan_set_flag( chan, ADMIN_ONLY );
    break;
  case "/wiz":
    chan_set_flag( chan, WIZ_ONLY );
    break;
  case "/readonly":
    chan_set_flag( chan, READ_ONLY );
    break;
  case "/permanent":
    chan_make_permanent( chan );
    break;
  case "":
  case "/who":
  case "/list":
    chan_who( chan );
    break;
  case "/delete":
    break;
  case "/info":
    break;
  default:
    if( strlen( cmd ) > 8 )
      if( cmd[..5] == "/color" ) {
	/* Let's change the color of the channel */
	chan_set_color( chan, cmd[7..] );
	break;
      } else if( cmd[..4] == "/imud" ) {
	chan_imud( chan, cmd[6..] );
	break;
      } else if( cmd[..5] == "/guild" ) {
	chan_set_guild( chan, cmd[7..] );
	break;
      }
    if( cmd[0] == ';' ) {
      chan_emote( chan, cmd[1..] );
      break;
    }
    chan_say( chan, cmd );
    return;
  }
}

void restore_me( void ) {
  restore_object( "/daemons/data/channel_d.o" );
}

void save_me( void ) {
  save_object( "/daemons/data/channel_d.o" );
}

void chan_set_flag( string chan, int flag ) {

  channels[chan] = flag;
  if( permanent[chan] )
    permanent[chan] = channels[chan];
  
  save_me();
}

void chan_make_permanent( string name ) {
  if( SECURE_D->query_admin( this_player()->query_name() ) != 1 ) {
    write( "Access denied.\n" );
    return;
  }

  permanent[name] = channels[name];
  save_me();
  write( "Channel " + name + " is permanent.\n" );
}

void chan_imud( string chan, string name ) {

  if( SECURE_D->query_admin( this_player()->query_name() ) != 1 ) {
    write( "Access denied.\n" );
    return;
  }

  if( !channels[chan] ) {
    write( "Channel doesn't exists.\n" );
    return;
  }

  if( !imud ) 
    imud = ([ ]);
  imud[name] = chan;
  save_me();
}

void chan_new( string name, int flags ) {

  name = lowercase( name );

  if( SECURE_D->query_wiz( this_player()->query_name() ) != 1 ) {
    write( "Access denied.\n" );
    return;
  }

  if( channels[name] ) {
    write( "Channel already exists.\n" );
    return;
  }
  channels[name] = flags;
  write( "Channel created.\n" );
}

void chan_join( string chan, object ob ) {

  chan = lowercase( chan );

  if( !channels[chan] ) {
    write( "No such channel." );
    return;
  }

  if( channels[chan] < READ_ONLY ) {
    if( SECURE_D->query_priv( ob->query_name() ) < channels[chan] - 1 ) {
      write( "No such channel.\n" );
      return;
    }
  }

  if( !guilds )
    guilds = ([ ]);

  if( guilds[chan] ) {
    if( !ob->guild_member( guilds[chan] ) ) {
      write( "No such channel.\n" );
      return;
    }
      
  }

  if( !listeners[chan] ) {
    listeners[chan] = ({ });
  }
  listeners[chan] -= ({ ob });
  listeners[chan] += ({ ob });
  write( "Subscribed to " + chan + ".\n" );
  ob->save_me();
}

void chan_leave( string chan, object ob ) {

  chan = lowercase( chan );

  if( !channels[chan] ) {
    write( "No such channel." );
    return;
  }

  if( channels[chan] != READ_ONLY )
    if( SECURE_D->query_priv( ob->query_name() ) < channels[chan] - 1 ) {
      write( "No such channel.\n" );
      return;
    }

  if( !listeners[chan] ) {
    listeners[chan] = ({ });
  }
  listeners[chan] -= ({ ob });

  write( "No longer subscribed to " + chan + ".\n" );

}

void chan_who( string chan ) {
  object *users;
  int i;

  chan = lowercase( chan );

  if( !listeners[chan] ) {
    write( "No subscribers." );
    return;
  }

  write( "Subscribed to " + chan + ":\n" );

  users = listeners[chan];
  for( i = 0; i < sizeof( users ); i ++ ) {
    write( "  " + capitalize( users[i]->query_name() ) + "\n" );
  }
}

void chan_send_string( string chan, string str ) {
  object *users;
  string line;
  int i;

  if( !colors ) {
    colors = ([ ]);
  }

  if( colors[chan] ) {
    line = colors[chan] + "[" + lowercase(chan) + "]%^RESET%^ " + str;
  } else {
    line = "[" + lowercase(chan) + "] " + str; 
  }
  users = listeners[chan];

  if( users )
    for( i = 0; i < sizeof( users ); i++ ) {
      users[i]->message( line );
    }
}

void chan_imud_rcv_targetted( string chan, string who, string where, 
			      string target, string targetmud, string what ) {
  if( !imud[chan] ) {
    return;
  }

  what = replace_string( what, "$O", target + "@" + targetmud );
  what = replace_string( what, "$N", who + "@" + where );
  chan_send_string( imud[chan], what );
}

void chan_imud_rcv_emote( string chan, string who, string where, 
			  string what ) {

  if( !imud[chan] ) {
    return;
  }
  what = replace_string( what, "$N", who + "@" + where );
  chan_send_string( imud[chan], what );
}

void chan_imud_rcv_say( string chan, string who, string where, string what ) {

  if( !imud[chan] ) {
     chan_send_string( "wiz", "[" + chan + "] " + who + "@" + where + ": " + what );
    return;
  }

  chan_send_string( imud[chan], who + "@" + where
		    + ": " + what );
}

void chan_emote( string chan, string what ) {
  string *ichans;
  string *result;
  int i;
  string cmd;
  string arg;
  
  if( !query_subscribed( chan ) ) {
    write( "Not subscribed to channel " + chan + ".\n" );
    return;
  }

  if( channels[chan] == READ_ONLY ) {
    write( chan+ " is read only." );
    return;
  }

  if( what == "" )
    return;

  if( sscanf( what, "%s %s", cmd, arg ) != 2 ) 
    cmd = what;
  
  if( EMOTE_D->is_emote( cmd ) ) {
    string *rules;
    string rule;
    object target;

    rules = EMOTE_D->query_rules( cmd );
    
    /* Targetted emote? Find the target */
    if( arg && arg != "" ) {
      target = this_environment()->find_object( arg );
      if( !target ) {
	target = USER_D->find_player( arg );
      }

    } else {
      target = nil;
    }
    if( target ) {
      /* We've found our target, check for the correct rule */

      if( target->is_living() ) {
	/* We're looking for a LIV rule */
	if( member_array( "LIV", rules ) != -1 ) {
	  rule = "LIV";
	} else {
	  rule = "";
	}
      } else {
	/* We're looking for a OBJ rule */
	if( member_array( "OBJ", rules ) != -1 ) {
	  rule = "OBJ";
	} else {
	  rule = "";
	}
      }
    } else {
      /* Or are we just looking for a string? */
      if( member_array( "STR", rules ) != -1  && arg != "" ) {
	rule = "STR";
      } else {
	rule = "";
      }
    }
    
    if( rule == "LIV" ) {
      result = compose_message( this_player(), EMOTE_D->query_emote( cmd, rule ), target, nil );
      what = result[2];
    } else if( rule == "OBJ" ) {
      result = compose_message( this_player(), EMOTE_D->query_emote( cmd, rule), nil, ({ target }) );
      what = result[2];
    } else if( rule == "STR" ) {
      result = compose_message( this_player(), EMOTE_D->query_emote( cmd, rule), nil, ({ arg }) );
      what = result[2];
    } else  {
      if( member_array( "", rules ) != -1 ) {
	result = compose_message( this_player(), EMOTE_D->query_emote( cmd, rule),nil, nil );
	what = result[2];
    } else
	write( "No such emote.\n" );
    }
  } else {
    if( arg && arg != "" )
      what = capitalize( this_player()->query_name() ) + " " + cmd + " " + arg;
    else
      what = capitalize( this_player()->query_name() ) + " " + cmd;
  }


#ifdef SYS_NETWORKING
  ichans = map_indices( imud );
  for( i = 0; i < sizeof( ichans ); i++ ) {
    if( imud[ichans[i]] == chan ) {

       what = replace_string( what, capitalize( this_player()->query_name() ), "$N" );
    
      /* Found an intermud channel */
      IMUD_D->do_channel_e( ichans[i], what );
      return;
    }
  }
#endif

  chan_send_string( chan, what );
}

void chan_say( string chan, string what ) {

  string *ichans;
  int i;

  if( !query_subscribed( chan ) ) {
    write( "Not subscribed to channel " + chan + ".\n" );
    return;
  }

  if( channels[chan] == READ_ONLY ) {
    write( chan+ " is read only." );
    return;
  }

  if( what == "" )
    return;

#ifdef SYS_NETWORKING
  ichans = map_indices( imud );
  for( i = 0; i < sizeof( ichans ); i++ ) {
    if( imud[ichans[i]] == chan ) {
      /* Found an intermud channel */
      IMUD_D->do_channel_m( ichans[i], what );
      return;
    }
  }
#endif
  
  chan_send_string( chan, capitalize( this_player()->query_name() ) + ": " + capitalize( what ) );
}

void chan_set_color( string chan, string col ) {

  if( SECURE_D->query_admin( this_player()->query_name() ) != 1 ) {
    write( "Access denied.\n" );
    return;
  }
  if( !colors )
    colors = ([ ]);
  colors[chan] = "%^" + uppercase( col ) + "%^";
  write( chan + " is now " + col + "\n" );
  save_me();
}

void chan_set_guild( string chan, string guild ) {

  if( SECURE_D->query_admin( this_player()->query_name() ) != 1 ) {
    write( "Access denied.\n" );
    return;
  }

  if( !guilds ) 
    guilds = ([ ]);

  guilds[chan] = guild;
  write( chan + " is now a " + guild + " only channel." );
  save_me();
}

int query_priv( string chan ) {
  return( channels[chan]-1 );
}

int query_channel( string chan ) {

  if( !guilds )
    guilds = ([ ]);

  if( channels[chan] ) {
    if( channels[chan] == READ_ONLY )
      return( 1 );
    if( guilds[chan] ) {
      /* Guild channel */
      if( SECURE_D->query_priv( this_player()->query_name() ) > 0 ) {
	/* A wiz can subscribe to all channels */
	return( 1 );
      } else {
	return( this_player()->guild_member( guilds[chan] ) );
      }
    }
    if( SECURE_D->query_priv( this_player()->query_name() ) >= channels[chan] - 1 )
      return( 1 );
    else 
      return( 0 );
  } else
    return( 0 );
}

string *query_channels( void ) {
   return map_indices(channels);
}

int query_subscribed( string chan ) {

  chan = lowercase( chan );
  if( listeners[chan] ) {
    if( member_array( this_player(), listeners[chan] ) != -1 ) {
      return( 1 );
    }
  }
  return( 0 );
}

void event_new_player( string *args ) {
  if( channels["announce"] ) {
    chan_send_string( "announce", "Trumpets sound as " + capitalize( args[0] ) + " joins the realm." );
  }
}

void event_player_login( string *args ) {
  if( channels["announce"] ) {
    chan_send_string( "announce", capitalize(args[0]) + " logs in." );
  }
}

void event_player_logout( string *args ) {
  if( channels["announce"] ) {
    chan_send_string( "announce", capitalize(args[0]) + " logs out." );
  }
}
 
void event_player_join( string *args ) {
  if( channels["announce"] ) {
    chan_send_string( "announce", "Trumpets blast a fanfare as " + capitalize(args[0]) + " joins " + args[1] + "!" );
  }
}
