/*|=====================================
 *|  Copyright (c) 2011-2013 hightoro 
 *|  All rights reserved                          
 *|=====================================
 */

using namespace std;

#include "stdinclude.hpp"
#include "option.hpp"
#include "GDS.hpp"

namespace GDS {

//
//  GDS::DATA
//
/* ----------------------- *
 *|  Init & Copy & Clear  |*
 * ----------------------- */
void DATA::init( )
{

}
void DATA::copy( const DATA& cp )
{
  RecordHeader_size1 = cp.RecordHeader_size1;
  RecordHeader_size2 = cp.RecordHeader_size2;
  RecordHeader_name  = cp.RecordHeader_name;
  RecordHeader_type  = cp.RecordHeader_type;  
  Data_list          = cp.Data_list;
}
void DATA::clear( )
{

}

/* -------- *
 *|  make  |* 
 * -------- */
// size //
int DATA::make_size( )
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 size;

  size.integer = Data_list.size()+4;

  RecordHeader_size1 = size.byte[1];
  RecordHeader_size2 = size.byte[0];

  return size.integer;
}
int DATA::make_size( int num )
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 size;

  size.integer = num+4;

  RecordHeader_size1 = size.byte[1];
  RecordHeader_size2 = size.byte[0];

  return size.integer;
}
// data(standard) //
void DATA::make_data_Byte2( short num )
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 data;

  data.integer = num;

  for( int i=1; i>=0; i--){
    Data_list.push_back( data.byte[i] );
  }
}
void DATA::make_data_Byte4( int num )
{
  union Byte4{
  public:
    int    integer;
    Byte   byte[4];
  };

  Byte4 data;

  data.integer = num;

  for( int i=3; i>=0; i--){
    Data_list.push_back( data.byte[i] );
  }
}
void DATA::make_data_String( string Str )
{
  for( unsigned int i=0; i<Str.size(); i++ ){
    Data_list.push_back(Str[i]);
  }

  if( (Str.size()%2)==1 ){
    Data_list.push_back(0x00);
  }
}
// data(special) //
void DATA::make_data_Header_5( )
{
  // v5.0
  Data_list.push_back(0x00);
  Data_list.push_back(0x05); 
}
void DATA::make_data_Header_6( )
{
  // v6.0
  Data_list.push_back(0x02);
  Data_list.push_back(0x56);
}
void DATA::make_data_CurrentTime( )
{
  //-----------------------------//
  time_t     current;
  struct tm  *local;  
  time(&current);
  local = localtime(&current);
  //-----------------------------// 

  Byte beginlib[12];
  beginlib[0]  = (Byte)(local->tm_year>>8);
  beginlib[1]  = (Byte)(local->tm_year);
  beginlib[2]  = (Byte)(local->tm_mon>>8);
  beginlib[3]  = (Byte)(local->tm_mon);
  beginlib[4]  = (Byte)(local->tm_mday>>8);
  beginlib[5]  = (Byte)(local->tm_mday);
  beginlib[6]  = (Byte)(local->tm_hour>>8);
  beginlib[7]  = (Byte)(local->tm_hour);
  beginlib[8]  = (Byte)(local->tm_min>>8);
  beginlib[9]  = (Byte)(local->tm_min);
  beginlib[10] = (Byte)(local->tm_sec>>8);
  beginlib[11] = (Byte)(local->tm_sec);
 
  for( int i=0; i<12; i++ ){
    Data_list.push_back( beginlib[i] );
  }
}
void DATA::make_data_Units( )
{
  Byte unit[16];
  unit[0]  = 0x3e;
  unit[1]  = 0x41;
  unit[2]  = 0x89;
  unit[3]  = 0x37;
  unit[4]  = 0x4b;
  unit[5]  = 0xc6;
  unit[6]  = 0xa7;
  unit[7]  = 0xef;
  unit[8]  = 0x39;
  unit[9]  = 0x44;
  unit[10] = 0xb8;
  unit[11] = 0x2f;
  unit[12] = 0xa0;
  unit[13] = 0x9b;
  unit[14] = 0x5a;
  unit[15] = 0x51;

  for( int i=0; i<16; i++ ){
    Data_list.push_back( unit[i] );
  }
}
void DATA::make_data_Angle( const string& angle )
{
  if( angle == "N" ){
    Data_list.push_back( 0x00 );
    Data_list.push_back( 0x00 );    
  }
  else if( angle == "FS" ){
    Data_list.push_back( 0x80 );
    Data_list.push_back( 0x00 );    
  }
}
void DATA::make_data_CenterMiddle( )
{
  Data_list.push_back( 0x00 );
  Data_list.push_back( 0x05 );    
}

/* ------- *
 *|  put  |* 
 * ------- */
void DATA::put_size( Byte size1, Byte size2 )
{
  RecordHeader_size1 = size1;
  RecordHeader_size2 = size2;
}
void DATA::put_name( Byte name )
{
  RecordHeader_name = name;
}
void DATA::put_type( Byte type )
{
  RecordHeader_type = type;
}
void DATA::put_data( Byte data )
{
  Data_list.push_back( data );
}

/* ------- *
 *|  get  |*
 * ------- */
short DATA::get_size( )const
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 size;

  size.byte[0] = RecordHeader_size2;
  size.byte[1] = RecordHeader_size1;

  return (size.integer-4);
}
string DATA::get_name( )const
{
  const Byte&  name_hex = RecordHeader_name;
  string name_string;

  switch( name_hex )
    {  
    case 0x00:
      name_string = "HEADER";
      break;
      
    case 0x01:
      name_string = "BGNLIB";
      break;
      
    case 0x02:
      name_string = "LIBNAME";
      break;
      
    case 0x03:
      name_string = "UNITS";
      break;
      
    case 0x04:
      name_string = "ENDLIB";
      break;
      
    case 0x05:
      name_string = "BGNSTR";
      break;
      
    case 0x06:
      name_string = "STRNAME";
      break;
      
    case 0x07:
      name_string = "ENDSTR";
      break;
      
    case 0x08:
      name_string = "BOUNDARY[EL]";
      break;
      
    case 0x09:
      name_string = "PATH";
      break;
      
    case 0x0a:
      name_string = "SREF";
      break;
      
    case 0x0c:
      name_string = "TEXT";
      break;
      
    case 0x0d:
      name_string = "LAYER";
      break;
      
    case 0x0e:
      name_string = "DATATYPE";
      break;
      
    case 0x10:
      name_string = "XY";
      break;
      
    case 0x11:
      name_string = "ENDEL";
      break;
      
    case 0x12:
      name_string = "SNAME";
      break;
      
    case 0x16:
      name_string = "TEXTTYPE";
      break;
      
    case 0x17:
      name_string = "PRESENTATION";
      break;
      
    case 0x19:
      name_string = "STRING";
      break;
      
    case 0x1a:
      name_string = "STRANS";
      break;
     
    case 0x1b:
      name_string = "MAG";
      break;
 
    default:
      name_string = "UNKNOWN";
    }

  return name_string;
}
string DATA::get_type( )const
{
  const Byte&  type_hex = RecordHeader_type;
  string type_string;

  switch( type_hex )
    {   
    case 0x00:
      type_string = "No_Data";
      break;
      
    case 0x01:
      type_string = "BitArray";
      break;
      
    case 0x02:
      type_string = "Integer_2";
      break;
      
    case 0x03:
      type_string = "Integer_4";
      break;
      
    case 0x04:
      type_string = "Real_4";
      break;
      
    case 0x05:
      type_string = "Real_8";
      break;
      
    case 0x06:
      type_string = "String";
      break;
      
    default:
      type_string = "UNKNOWN";
    }

  return type_string;
}
short DATA::get_data_Header( ) const
{
  /*
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 data;

  data.byte[1];
  data.byte[0];

  BOOST_FOREACH( Byte data_Byte, Data_list ){
    data_string += data_Byte;
  }
  */

  return 5;
}
string DATA::get_data_String( )const
{
  string data_string;

  BOOST_FOREACH( Byte data, Data_list ){
    data_string += data;
  }

  return data_string;
}


/* -------- *
 *|  show  |*
 * -------- */
void DATA::show_size( )const
{
  int size = 4;  size += get_size();
  cout << boost::format("size :%3d  ")%size << flush;
}
void DATA::show_name( )const
{
  cout << boost::format("(0x%02x)%-15s ")%(0xff & RecordHeader_name) %get_name() << flush;
}
void DATA::show_type( )const
{
  cout << boost::format("(0x%02x)%-15s ")%(0xff & RecordHeader_type) %get_type() << flush;
} 
void DATA::show_data( )const
{
  switch( RecordHeader_type ){

  // Integer 2
  case Integer_2 :
    show_data_Integer2();
    break;
  // Integer 4
  case Integer_4 :
    show_data_Integer4();
    break;
  // String
  case String :
    show_data_String();
    break;
  // Other
  default :
    show_data_Other();
  }
}
void DATA::show_data_Integer2( )const
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 data;

  for( auto i=Data_list.begin(); i!=Data_list.end(); ){
    for( int j=1; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    cout << boost::format("[%4d]")%(data.integer) << flush;
  }

}
void DATA::show_data_Integer4( )const
{
  union Byte4{
  public:
    int    integer;
    Byte   byte[4];
  };

  Byte4 data;
 
  for( auto i=Data_list.begin(); i!=Data_list.end(); ){
    for( int j=3; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    cout << boost::format("[%4d]")%(data.integer) << flush;
  }
}
void DATA::show_data_String( )const
{
  for( auto& data : Data_list ){
    cout << boost::format("[%c]")%(data) << flush;
  }
}
void DATA::show_data_Other( )const
{
  for( auto& data : Data_list ){
    cout << boost::format("%02x ")%( (0xff)&(data) ) << flush;
  }
}
/* --------- *
 *|  Print  |*
 * --------- */
void DATA::print( )const
{
  // RecordHeader
  show_size( );
  show_name( );
  show_type( );
  cout << endl;

  // Data
  show_data( );
  cout << endl;

  cout << endl;
}

/* -------- *
 *|  Read  |*
 * -------- */
void DATA::read_gdsii( byte_ifstream& file )
{
  Byte      RecordHeader[4];
  Byte      Data[1];

  // RecordHeader
  file.read( RecordHeader, sizeof(Byte)*4 );
  put_size( RecordHeader[0], RecordHeader[1] );
  put_name( RecordHeader[2] );
  put_type( RecordHeader[3] );
  
  // Data
  int size = get_size();
  for( int i=0; i<size; ++i ){
    file.read( Data, sizeof(Byte) );
    put_data( Data[0] );
  }
}

/* --------- *
 *|  Write  |*
 * --------- */
void DATA::write_gdsii( ofstream& file )const
{
  // RecordHeader
  file << RecordHeader_size1;
  file << RecordHeader_size2;
  file << RecordHeader_name;
  file << RecordHeader_type;

  // Data
  for( auto& data : Data_list ){
    file << data;
  }
}



/* -------------------- *
 *|  produce (static)  |*
 * -------------------- */
// 0x00
DATA product_NoData( Byte name )
{
  Byte  type = 0x00;
  DATA  gds(name,type);

  gds.make_size( );

  return gds;
}
// 0x01
DATA product_STRANS( Byte name, const string& angle )
{
  Byte  type = 0x01;
  DATA  gds(name,type);

  gds.make_data_Angle( angle );
  gds.make_size( );

  return gds;
}
DATA product_CenterMiddle( Byte name )
{
  Byte  type = 0x01;
  DATA  gds(name,type);

  gds.make_data_CenterMiddle( );
  gds.make_size( );

  return gds;
}
// 0x02
DATA product_Integer2( Byte name, short num )
{
  Byte  type = 0x02;
  DATA  gds(name,type);

  gds.make_data_Byte2( num );
  gds.make_size( );

  return gds;
}
DATA product_Integer2( Byte name, const vector<short>& array )
{
  Byte  type = 0x02;
  DATA  gds(name,type);

  BOOST_FOREACH( short num, array ){
    gds.make_data_Byte2( num );
  }
  gds.make_size( );

  return gds;
}
DATA product_Integer2_Coord( Byte name, short x, short y )
{
  Byte type = 0x02;
  DATA  gds(name,type);

  gds.make_data_Byte2( x );  gds.make_data_Byte2( y );
  gds.make_size( );

  return gds;
}
DATA product_Integer4( Byte name, const vector<int>& array )
{
  Byte  type = 0x03;
  DATA  gds(name,type);

  BOOST_FOREACH( int num, array ){
    gds.make_data_Byte4( num );
  }
  gds.make_size( );

  return gds;
}
DATA product_Integer4_Coord( Byte name, int X, int Y )
{
  Byte  type = 0x03;
  DATA  gds(name,type);

  gds.make_data_Byte4( X ); gds.make_data_Byte4( Y );
  gds.make_size( );

  return gds;
}
DATA product_Integer4_Rect( Byte name, int x1, int y1, int x2, int y2 )
{
  Byte  type = 0x03;
  DATA  gds(name,type);

  gds.make_data_Byte4( x1 );  gds.make_data_Byte4( y1 );
  gds.make_data_Byte4( x1 );  gds.make_data_Byte4( y2 );
  gds.make_data_Byte4( x2 );  gds.make_data_Byte4( y2 );
  gds.make_data_Byte4( x2 );  gds.make_data_Byte4( y1 );
  gds.make_data_Byte4( x1 );  gds.make_data_Byte4( y1 );
  gds.make_size( );

  return gds;
}
DATA product_Integer4_Array( Byte name, int point_x, int point_y, int max_x, int max_y )
{
  Byte  type = 0x03;
  DATA  gds(name,type);

  gds.make_data_Byte4( point_x ); gds.make_data_Byte4( point_y );
  gds.make_data_Byte4( max_x );   gds.make_data_Byte4( point_y );
  gds.make_data_Byte4( point_x ); gds.make_data_Byte4( max_y );
  gds.make_size( );

  return gds;
}
DATA product_String( Byte name, const string& word )
{
  Byte  type = 0x06;
  DATA  gds(name,type);

  gds.make_data_String( word );
  gds.make_size( );

  return gds;
}
// BGN,END,NAME
DATA product_BGN( Byte name )
{
  Byte  type = 0x02;
  DATA  gds(name,type);

  gds.make_data_CurrentTime( );
  gds.make_data_CurrentTime( );
  gds.make_size( );

  return gds;
}
DATA product_END( Byte name )
{
  Byte  type = 0x00;
  DATA  gds(name,type);

  gds.make_size( );

  return gds;
}
DATA product_NAME( Byte name, const string& name_string )
{
  Byte  type = 0x06;
  DATA  gds(name,type);

  gds.make_data_String( name_string );
  gds.make_size( );

  return gds;
}
// STR //
DATA product_STRBGN( )
{
  Byte name = 0x05;
  return product_BGN( name );
}
DATA product_STRNAME( const string& name_string )
{
  Byte name = 0x06;
  return product_NAME( name, name_string  ); 
}
DATA product_STREND( )
{
  Byte name = 0x07;
  return product_END( name ); 
}
// LIB //
DATA product_HEADER( )
{
  Byte name = 0x00;
  Byte type = 0x02;
  DATA gds(name,type);

  gds.make_data_Header_5( );
  gds.make_size( );
  return gds;
}
DATA product_LIBBGN( )
{
  Byte name = 0x01;
  return product_BGN( name );
}
DATA product_LIBNAME( const string& name_string )
{
  Byte name = 0x02;
  return product_NAME( name, name_string  ); 
}
DATA product_UNITS( )
{
  Byte name = 0x03;
  Byte type = 0x03;
  DATA gds(name,type);

  gds.make_data_Units( );
  gds.make_size( );

  return gds;
}
DATA product_LIBEND( )
{
  Byte name = 0x04;
  return product_END( name ); 
}


//
//  GDS::STR
// 
/* ----------------------- *
 *|  Init & Copy & Clear  |*
 * ----------------------- */
void STR::init( )
{

}
void STR::copy( const STR& cp )
{
  /* GDS */
  STR_bgn     = cp.STR_bgn;
  STR_name    = cp.STR_name;

  GDS_stream  = cp.GDS_stream;           //

  STR_end     = cp.STR_end;
}
void STR::clear( )
{

}

/* -------- *
 *|  make  |*
 * -------- */
void STR::make_BOUNDARY( short layer, short data, int x1, int y1, int x2, int y2 )
{
  append_BOUNDARY( GDS_stream, layer, data, x1, y1, x2, y2 );
}
void STR::make_SREF( const string& sname, const string& orient, int x, int y )
{
  append_SREF( GDS_stream, sname, orient, x, y );
}
void STR::make_TEXT( const string& pname, short layer, short type, const string& orient, int x, int y )
{
  append_TEXT( GDS_stream, pname, layer, type, orient, x, y );
}

/* ------- *
 *|  put  |*
 * ------- */
void STR::put_name( const string& name )
{
  STR_name = product_STRNAME( name );
}
void STR::put_stream( const vector<DATA>& stream )
{
  for( auto& gds : stream ){
    GDS_stream.push_back(gds);
  }
}

/* ------- *
 *|  get  |*
 * ------- */
string STR::get_name( )const
{
  return STR_name.get_data_String();
}
vector<DATA> STR::get_stream( )const
{
  return GDS_stream;
}

/* --------- *
 *|  Print  |*
 * --------- */
void STR::print( )const
{ 
  STR_bgn.print( );
  STR_name.print( );
  
  for( auto& gds : GDS_stream ){
    gds.print( );
  } 

  STR_end.print( );
}

/* -------- *
 *|  Read  |*
 * -------- */
void STR::read_gdsii( byte_ifstream& file )
{
  while( !file.eof() ) {
    DATA gds( file );

    parse_gdsii( gds );

    if( gds.get_name() == "ENDSTR" )
      break;
  }
}

/* --------- *
 *|  parse  |*
 * --------- */
void STR::parse_gdsii( const DATA& gds )
{
  string name = gds.get_name();

  // BGNSTR  (0x05)
  if( name == "BGNSTR" )
    STR_bgn = gds;
  
  // STRNAME (0x06)
  else if( name == "STRNAME" ){
    //name     = gds.get_data_String();
    STR_name = gds;
  }  
  // ENDSTR  (0x07)
  else if( name == "ENDSTR" )
    STR_end = gds;

  // OTHER
  else	    
    GDS_stream.push_back( gds );
}

/* --------- *
 *|  Write  |*
 * --------- */
void STR::write_gdsii( ofstream& file )const
{
  STR_bgn.write_gdsii( file );
  STR_name.write_gdsii( file );
  
  for( auto& gds : GDS_stream ){
    gds.write_gdsii( file );
  } 

  STR_end.write_gdsii( file );
}


//
//  GDS::LIB
// 
/* ----------------------- *
 *|  Init & Copy & Clear  |*
 * ----------------------- */
void LIB::init( )
{

}
void LIB::copy( const LIB& cp )
{
  LIB_hdr     = cp.LIB_hdr;
  LIB_bgn     = cp.LIB_bgn;
  LIB_name    = cp.LIB_name;
  LIB_units   = cp.LIB_units;

  STR_stream  = cp.STR_stream;           //

  LIB_end     = cp.LIB_end;
}
void LIB::clear( )
{

}

/* ------- *
 *|  put  |*
 * ------- */
void LIB::put_name( const string& name )
{
  LIB_name = product_LIBNAME(name);
}
void LIB::put_stream( const STR& stream )
{
  STR_stream.push_back(stream);
}
void LIB::put_stream( const vector<STR>& stream )
{
  for( auto& gds_str : stream ){
    STR_stream.push_back(gds_str);
  }
}

/* ------- *
 *|  get  |*
 * ------- */
short LIB::get_ver( )
{
  return LIB_hdr.get_data_Header();
}
string LIB::get_name( )
{
  return LIB_name.get_data_String();
}
vector<STR> LIB::get_stream( )
{
  return STR_stream;
}

/* --------- *
 *|  Print  |*
 * --------- */
void LIB::print( )const
{ 
  LIB_hdr.print( );
  LIB_bgn.print( );
  LIB_name.print( );
  LIB_units.print( );
  
  for( auto& gds_str : STR_stream ){
    gds_str.print( );
  } 

  LIB_end.print( );
}

/* -------- *
 *|  Read  |*
 * -------- */
void LIB::read_gdsii( const string& f_name )
{
  byte_ifstream file;
  read_gdsii( file_open(file,f_name) );
}
void LIB::read_gdsii( byte_ifstream& file )
{
  while( !file.eof() ) {
    DATA gds( file );

    gds.print();
    parse_gdsii( gds );

    if( gds.get_name() == "ENDLIB" )
      break;
  }
}
byte_ifstream& LIB::file_open( byte_ifstream& file, const string& f_name ) 
{
  try{
    file.open(f_name.c_str(),ios::in|ios::binary);
    if(!file.is_open()) throw logic_error("file is not found");
    return file;
  }
  catch( const exception& e ) {
    cerr << "file is not found" << endl;
    exit(1);
  }
}
/* --------- *
 *|  parse  |*
 * --------- */
void LIB::parse_gdsii( const DATA& gds )
{
  string name = gds.get_name();

  // HEADRE  (0x00)
  if( name == "HEADER" )
    LIB_hdr = gds;
  
  // BGNLIB  (0x01)
  else if( name == "BGNLIB" )
    LIB_bgn = gds;
  
  // LIBNAME (0x02)
  else if( name == "LIBNAME" )
    LIB_name = gds;
  
  // Units   (0x03)
  else if( name == "UNITS" )
    LIB_units = gds;
  
  // ENDLIB  (0x04)
  else if( name == "ENDLIB" )
    LIB_end = gds;

  // BGNSTR  (0x05)
  else if( name == "BGNSTR" ){
    STR  gds_str( gds ); 
    STR_stream.push_back( gds_str );
  }
  // OTHER
  else{
    STR& gds_str = STR_stream.back();
    gds_str.parse_gdsii( gds );
  }
}

/* --------- *
 *|  Write  |*
 * --------- */
void LIB::write_gdsii( ofstream& file )
{
  LIB_hdr.write_gdsii( file );
  LIB_bgn.write_gdsii( file );
  LIB_name.write_gdsii( file );
  LIB_units.write_gdsii( file );
  
  for( auto& gds_str : STR_stream ){
    gds_str.write_gdsii( file );
  } 

  LIB_end.write_gdsii( file );
}

/* ------------------- *
 *|  Append (static)  |*
 * ------------------- */
void append_BOUNDARY( vector<DATA>& stream,short layer_num,short data_num,int coord1_x,int coord1_y,int coord2_x,int coord2_y )
{
  DATA        gds[5];
  Byte        name;

  // 08.BOUNDARY(begin)
  name=0x08;
  gds[0]=product_NoData( name );
  stream.push_back( gds[0] );
  // 08.BOUNDARY(end)
    
  // 0d.LAYER(begin)
  name=0x0d;
  gds[1]=product_Integer2( name, layer_num );
  stream.push_back( gds[1] );
  // 0d.LAYER(end)
    
  // 0e.DATATYPE(begin)
  name=0x0e;
  gds[2]=product_Integer2( name, data_num );
  stream.push_back( gds[2] );
  // 0e.DATATYPE(end)

  // 10.XY(begin)
  name=0x10;
  gds[3]=product_Integer4_Rect( name, coord1_x, coord1_y, coord2_x, coord2_y );
  stream.push_back( gds[3] );
  // 10.XY(end)
  
  // 11.ENDEL(begin)
  name=0x11;
  gds[4]=product_NoData( name );
  stream.push_back( gds[4] );
  // 11.ENDEL(end)
}
void append_SREF( vector<DATA>& stream,const string& sname,const string& angle,int coord_x,int coord_y )
{
  DATA        gds[5];
  Byte        name;

  // 0a.SREF(begin)
  name=0x0a;
  gds[0]=product_NoData( name );
  stream.push_back( gds[0] );
  // 0a.SREF(end)
    
  // 12.SNAME(begin)
  name=0x12;
  gds[1]=product_String( name, sname );
  stream.push_back( gds[1] );
  // 12.SNAME(end)
    
  // 1a.STRANS(begin)
  name=0x1a;
  gds[2]=product_STRANS( name, angle );
  stream.push_back( gds[2] );
  // 1a.STRANS(end)

  // 10.XY(begin)
  name=0x10;
  gds[3]=product_Integer4_Coord( name, coord_x, coord_y );
  stream.push_back( gds[3] );
  // 10.XY(end)
  
  // 11.ENDEL(begin)
  name=0x11;
  gds[4]=product_NoData( name );
  stream.push_back( gds[4] );
  // 11.ENDEL(end)
}
void append_AREF( vector<DATA>& stream, const string& sname, const string& angle,int coord_x,int coord_y,short size_x,short size_y,int max_x,int max_y )
{
  DATA        gds[6];
  Byte        name;

  // 0b.AREF(begin)
  name=0x0a;
  gds[0]=product_NoData( name );
  stream.push_back( gds[0] );
  // 0b.AREF(end)
    
  // 12.SNAME(begin)
  name=0x12;
  gds[1]=product_String( name, sname );
  stream.push_back( gds[1] );
  // 12.SNAME(end)
    
  // 1a.STRANS(begin)
  name=0x1a;
  gds[2]=product_STRANS( name, angle );
  stream.push_back( gds[2] );
  // 1a.STRANS(end)

  // 13.COLROW(begin)
  name=0x13;
  gds[3]=product_Integer2_Coord( name, size_x, size_y );
  stream.push_back( gds[3] );
  // 13.CORLOW(end)

  // 10.XY(begin)
  name=0x10;
  gds[4]=product_Integer4_Array( name, coord_x, coord_y, max_x, max_y );
  stream.push_back( gds[4] );
  // 10.XY(end)
  
  // 11.ENDEL(begin)
  name=0x11;
  gds[5]=product_NoData( name );
  stream.push_back( gds[5] );
  // 11.ENDEL(end)
}
void append_TEXT( vector<DATA>& stream, const string& pname,short layer_num,short text_type,const string& angle,int coord_x,int coord_y )
{
  DATA        gds[9];
  Byte        name;

  // 0c.TEXT(begin)
  name=0x0c;
  gds[0]=product_NoData( name );
  stream.push_back( gds[0] );
  // 0c.TEXT(end)
    
  // 0d.LAYER(begin)
  name=0x0d;
  gds[1]=product_Integer2( name, layer_num );
  stream.push_back( gds[1] );
  // 0d.LAYER(end)
    
  // 16.TEXTTYPE(begin)
  name=0x16;
  gds[2]=product_Integer2( name, text_type );
  stream.push_back( gds[2] );
  // 16.TEXTTYPE(end)

  // 17.PRESENTATION(begin)
  name=0x17;
  gds[3]=product_CenterMiddle( name );
  stream.push_back( gds[3] );
  // 17.PRESENTATION(end)

  // 1a.STRANS(begin)
  name=0x1a;
  gds[4]=product_STRANS( name, angle );
  stream.push_back( gds[4] );
  // 1a.STRANS(end)

  // 10.XY(begin)
  name=0x10;
  gds[5]=product_Integer4_Coord( name, coord_x, coord_y );
  stream.push_back( gds[5] );
  // 10.XY(end)
  
  // 19.STRING(begin)
  name=0x19;
  gds[6]=product_String( name, pname );
  stream.push_back( gds[6] );
  // 19.STRING(end)

  // 11.ENDEL(begin)
  name=0x11;
  gds[7]=product_NoData( name );
  stream.push_back( gds[7] );
  // 11.ENDEL(end)
}

}
