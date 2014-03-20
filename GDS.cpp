/*|=====================================
 *|  Copyright (c) 2011-2013 hightoro 
 *|  All rights reserved                          
 *|=====================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include <boost/format.hpp>

#include "GDS.hpp"

namespace GDS {

//
//  GDS::DATA
//

/* ------- *
 *|  put  |* 
 * ------- */
void DATA::put_name( Record_type name )
{
  RecordHeader_name_ = static_cast<Record_type>(name);
}
void DATA::put_type( Data_type type )
{
  RecordHeader_type_ = static_cast<Data_type>(type);
}
void DATA::put_data_Integer2( short num )
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 data;

  data.integer = num;

  for( int i=1; i>=0; i--){
    Data_list_.push_back( data.byte[i] );
  }
}
void DATA::put_data_String( const std::string& word )
{
  for( unsigned int i=0; i<word.size(); i++ ){
    Data_list_.push_back(word[i]);
  }

  if( (word.size()%2)==1 ){
    Data_list_.push_back(0x00);
  }
}

  ///
void DATA::put_size( Byte size1, Byte size2 )
{
  RecordHeader_size1_ = size1;
  RecordHeader_size2_ = size2;
}
void DATA::put_name( Byte name )
{
  RecordHeader_name_ = static_cast<Record_type>(name);
}
void DATA::put_type( Byte type )
{
  RecordHeader_type_ = static_cast<Data_type>(type);
}
void DATA::put_data( Byte data )
{
  Data_list_.push_back( data );
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

  size.byte[0] = RecordHeader_size2_;
  size.byte[1] = RecordHeader_size1_;

  return std::move(size.integer-4);
}
std::string DATA::get_name( )const
{
  std::string name_string;

  switch( RecordHeader_name_ )
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
      name_string = "PATH[EL]";
      break;
      
    case 0x0a:
      name_string = "SREF[EL]";
      break;

    case 0x0b:
      name_string = "AREF[EL]";
      break;      

    case 0x0c:
      name_string = "TEXT[EL]";
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

  return std::move(name_string);
}
std::string DATA::get_type( )const
{
  std::string type_string;

  switch( RecordHeader_type_ )
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

  return std::move(type_string);
}
std::vector<short> DATA::get_data_Integer2( )const
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  std::vector<short> value_list;

  for( auto i=Data_list_.begin(); i!=Data_list_.end(); ){
    Byte2 data;
    for( int j=1; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    value_list.push_back(std::move(data.integer));
  }

  return std::move(value_list);
}
std::vector<int> DATA::get_data_Integer4( )const
{
  union Byte4{
  public:
    int    integer;
    Byte   byte[4];
  };

  std::vector<int> value_list;
 
  for( auto i=Data_list_.begin(); i!=Data_list_.end(); ){
    Byte4 data;
    for( int j=3; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    value_list.push_back(std::move(data.integer));
  }

  return std::move(value_list);
}
std::string DATA::get_data_String( )const
{
  std::string data_string;

  for( auto& d : Data_list_ ){
    data_string += d;
  }

  return std::move(data_string);
}

/* --------- *
 *|  other  |*
 * --------- */
short DATA::resize()
{
  union Byte2{
  public:
    short  integer;
    Byte   byte[2];
  };

  Byte2 size;

  size.integer = Data_list_.size()+4;

  RecordHeader_size1_ = size.byte[1];
  RecordHeader_size2_ = size.byte[0];

  return size.integer;
}

/* -------- *
 *|  show  |*
 * -------- */
void DATA::show_size( )const
{
  int size = 4;  size += get_size();
  std::cout << boost::format("size :%3d  ")%size << std::flush;
}
void DATA::show_name( )const
{
  std::cout << boost::format("(0x%02x)%-15s ")%(0xff & RecordHeader_name_) %get_name() << std::flush;
}
void DATA::show_type( )const
{
  std::cout << boost::format("(0x%02x)%-15s ")%(0xff & RecordHeader_type_) %get_type() << std::flush;
} 
void DATA::show_data( )const
{
  switch( RecordHeader_type_ ){

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

  for( auto i=Data_list_.begin(); i!=Data_list_.end(); ){
    for( int j=1; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    //std::cout << boost::format("[%4d]")%(data.integer) << std::flush;
    std::cout << "[" << data.integer << "]" << std::flush;
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
 
  for( auto i=Data_list_.begin(); i!=Data_list_.end(); ){
    for( int j=3; j>=0; j--,i++){
      data.byte[j] = (*i);
    }
    //std::cout << boost::format("[%4d]")%(data.integer) << std::flush;
    std::cout << "[" << data.integer << "]" << std::flush;
  }
}
void DATA::show_data_String( )const
{
  for( auto& data : Data_list_ ){
    std::cout << "[" << data << "]" << std::flush;
  }
}
void DATA::show_data_Other( )const
{
  for( auto& data : Data_list_ ){
    std::cout << ((0xff)&(data)) << " " << std::flush;
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
  std::cout << std::endl;

  // Data
  show_data( );
  std::cout << std::endl;

  std::cout << std::endl;
}

/* -------- *
 *|  Read  |*
 * -------- */
void DATA::read_gdsii( std::ifstream& file )
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
  void DATA::write_gdsii( std::ofstream& file )const
{
  // RecordHeader
  file << RecordHeader_size1_;
  file << RecordHeader_size2_;
  file << RecordHeader_name_;
  file << RecordHeader_type_;

  // Data
  for( auto& data : Data_list_ ){
    file << data;
  }
}

/* -------- *
 *|  make  |* 
 * -------- */
/*
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
*/

//
//  GDS::EL
//
/* ------- *
 *|  put  |*
 * ------- */
void EL::put_DATA( DATA&& data )
{
  EL_list_.push_back( std::move(data) );
}
void EL::put_DATA( std::vector<DATA>&& data_list )
{
  std::move( data_list.begin(), data_list.end(), std::back_inserter(EL_list_) );
}

/* ------- *
 *|  get  |*
 * ------- */
std::string EL::get_type( )const
{
  return EL_bgn_.get_type();
}

/* --------- *
 *|  Print  |*
 * --------- */
void EL::print( )const
{ 
  EL_bgn_.print( );
  
  for( auto& gds : EL_list_ ){ gds.print( ); } 

  EL_end_.print( );
}

/* --------- *
 *|  parse  |*
 * --------- */
void EL::parse_gdsii( DATA&& gds )
{
  switch( gds.get_record() ){

  // BOUNDARY  (0x08)
  case Record_type::BOUNDARY :
    EL_bgn_ = std::move(gds);
    break;

  // PATH  (0x09)
  case Record_type::PATH :
    EL_bgn_ = std::move(gds);
    break;
    
  // SREF  (0x0a)
  case Record_type::SREF :
    EL_bgn_ = std::move(gds);
    break;

  // AREF  (0x0b)
  case Record_type::AREF :
    EL_bgn_ = std::move(gds);
    break;

  // TEXT  (0x0c)
  case Record_type::TEXT :
    EL_bgn_ = std::move(gds);
    break;

  // ENDEL  (0x11)
  case Record_type::ENDEL :
    EL_end_ = std::move(gds);
    break;

  // OTHER
  default : 
    EL_list_.push_back( std::move(gds) );

  }
}

/* --------- *
 *|  Write  |*
 * --------- */
void EL::write_gdsii( std::ofstream& file )const
{
  EL_bgn_.write_gdsii( file );
 
  for( auto& gds : EL_list_ ){ gds.write_gdsii( file ); } 

  EL_end_.write_gdsii( file );
}

//
//  GDS::STR
// 
/* ------- *
 *|  put  |*
 * ------- */
void STR::put_name( const std::string& name )
{
  STR_name_ = product_STRNAME( name );
}
void STR::put_EL( std::vector<EL>&& el_list )
{
  for( auto& gds : el_list ){ STR_list_.push_back(gds); }
}

/* ------- *
 *|  get  |*
 * ------- */
std::string STR::get_name( )const
{
  return STR_name_.get_data_String();
}

/* --------- *
 *|  Print  |*
 * --------- */
void STR::print( )const
{ 
  STR_bgn_.print( );
  STR_name_.print( );
  
  for( auto& gds : STR_list_ ){ gds.print( ); } 

  STR_end_.print( );
}

/* --------- *
 *|  parse  |*
 * --------- */
void STR::parse_gdsii( DATA&& gds )
{
  switch( gds.get_record() ){

  // BGNSTR  (0x05)
  case Record_type::BGNSTR :
    STR_bgn_ = std::move(gds);
    break;  

  // STRNAME (0x06)
  case Record_type::STRNAME :
    STR_name_ = std::move(gds);
    break;  

  // ENDSTR  (0x07)
  case Record_type::ENDSTR :
    STR_end_ = std::move(gds);
    break;

  // BOUNDARY  (0x08)
  case Record_type::BOUNDARY :
    STR_list_.emplace_back( std::move(gds) );
    break;

  // PATH  (0x09)
  case Record_type::PATH :
    STR_list_.emplace_back( std::move(gds) );
    break;

  // SREF  (0x0a)
  case Record_type::SREF :
    STR_list_.emplace_back( std::move(gds) );
    break;

  // AREF  (0x0b)
  case Record_type::AREF :
    STR_list_.emplace_back( std::move(gds) );
    break;
    
  // TEXT  (0x0c)
  case Record_type::TEXT  :
    STR_list_.emplace_back( std::move(gds) );
    break;

  // OTHER
  default :
    STR_list_.push_back( std::move(gds) );
  }
}

/* --------- *
 *|  Write  |*
 * --------- */
void STR::write_gdsii( std::ofstream& file )const
{
  STR_bgn_.write_gdsii( file );
  STR_name_.write_gdsii( file );
  
  for( auto& gds : STR_list_ ){ gds.write_gdsii( file ); } 

  STR_end_.write_gdsii( file );
}

/* -------- *
 *|  make  |*
 * -------- */
/*
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
*/


//
//  GDS::LIB
// 
/* ------- *
 *|  put  |*
 * ------- */
void LIB::put_version( short ver )
{
  LIB_hdr_ = product_HEADER(ver); 
}
void LIB::put_name( const std::string& name )
{
  LIB_name_ = product_LIBNAME(name);
}
void LIB::put_STR( STR&& str )
{
  LIB_list_.push_back( std::move(str) );
}
void LIB::put_STR( std::vector<STR>&& str_list )
{
  std::move( str_list.begin(),str_list.end(),std::back_inserter(LIB_list_) );
}

/* ------- *
 *|  get  |*
 * ------- */
short LIB::get_version( )const
{
  return LIB_hdr_.get_data_Integer2()[0];
}
std::string LIB::get_name( )const
{
  return LIB_name_.get_data_String();
}

/* --------- *
 *|  Print  |*
 * --------- */
void LIB::print( )const
{ 
  LIB_hdr_.print( );
  LIB_bgn_.print( );
  LIB_name_.print( );
  LIB_units_.print( );
  
  for( auto& str : LIB_list_ ){ str.print( ); } 

  LIB_end_.print( );
}

/* -------- *
 *|  Read  |*
 * -------- */
void LIB::read_gdsii( std::ifstream& file )
{
  while( !file.eof() ) {
    parse_gdsii( DATA(file) );
    if( LIB_end_.get_record() == Record_type::ENDLIB ){ break; }
  }
}

/* --------- *
 *|  parse  |*
 * --------- */
void LIB::parse_gdsii( DATA&& gds )
{
  switch( gds.get_record() ){

  // HEADRE  (0x00)
  case Record_type::HEADER :
    LIB_hdr_   = std::move(gds);
    break;

  // BGNLIB  (0x01)
  case Record_type::BGNLIB :
    LIB_bgn_   = std::move(gds);
    break;  

  // LIBNAME (0x02)
  case Record_type::LIBNAME :
    LIB_name_  = std::move(gds);
    break;

  // Units   (0x03)
  case Record_type::UNITS :
    LIB_units_ = std::move(gds);
    break;  

  // ENDLIB  (0x04)
  case Record_type::ENDLIB :
    LIB_end_   = std::move(gds);
    return;

  // BGNSTR  (0x05)
  case Record_type::BGNSTR :
    LIB_list_.emplace_back( std::move(gds) );
    break;

  // OTHER
  default : 
    LIB_list_.back().parse_gdsii( std::move(gds) );
   
  }
}
/* --------- *
 *|  Write  |*
 * --------- */
void LIB::write_gdsii( std::ofstream& file )const
{
  LIB_hdr_.write_gdsii( file );
  LIB_bgn_.write_gdsii( file );
  LIB_name_.write_gdsii( file );
  LIB_units_.write_gdsii( file );
  
  for( auto& str : LIB_list_ ){
    str.write_gdsii( file );
  } 

  LIB_end_.write_gdsii( file );
}

/* ------------------- *
 *|  Append (static)  |*
 * ------------------- */
/*
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
*/

/* -------------------- *
 *|  produce (static)  |*
 * -------------------- */
DATA product_HEADER( short ver )
{
  Byte name = 0x00;
  Byte type = 0x02;

  DATA gds(name,type);
  gds.put_data_Integer2( ver );
  gds.resize( );

  return gds;
}
  DATA product_NAME( Record_type name, const std::string& name_string )
{
  Data_type type = Data_type::String;
  return DATA(name,type,name_string);
}
DATA product_LIBNAME( const std::string& name_string )
{
  Record_type name = Record_type::LIBNAME;
  return product_NAME( name, name_string  ); 
}
DATA product_STRNAME( const std::string& name_string )
{
  Record_type name = Record_type::STRNAME;
  return product_NAME( name, name_string  ); 
}


/*
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
*/

/*
static std::ifstream LIB::file_open( const std::string& f_name ) 
{
  std::ifstream file(f_name,ios::in|ios::binary);
  if(!file.is_open()) throw logic_error(f_name);
  
  return std::move(file);
}
*/

}
