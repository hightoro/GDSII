/*|=====================================
 *|  Copyright (c) 2011-2013 hightoro
 *|  All rights reserved
 *|=====================================
 */
#ifndef CLASS_GDS_H
#define CLASS_GDS_H

#include <fstream>
#include <string>
#include <vector>

namespace GDS {

  class LIB;   // Library
  class STR;   //  -Structure ( some of EL )
  class EL;    //   -Element ( some of Record )
  class DATA;  //    -Record ( 4 bytes +  )
//class byte;  //      -bytes( 8bit data )


//
//  GDS::Byte
//
typedef char Byte;

//
//  GDS::Record_type
//
enum Record_type : Byte  // ( Data_type )
{
  HEADER      =0x00,  // ( INTEGER2 )
  BGNLIB      =0x01,  // ( INTEGER2 )
  LIBNAME     =0x02,  // ( String )
  UNITS       =0x03,  // ( Real8 )
  ENDLIB      =0x04,  // ( NoData )
  BGNSTR      =0x05,  // ( INTEGER2 )
  STRNAME     =0x06,  // ( String )
  ENDSTR      =0x07,  // ( NoData )
  BOUNDARY    =0x08,  // ( NoData )
  PATH        =0x09,  // ( NoData )
  SREF        =0x0a,  // ( NoData )
  AREF        =0x0b,  // ( NoData )
  TEXT        =0x0c,  // ( NoData )
  LAYER       =0x0d,  // ( INTEGER2 )
  DATATYPE    =0x0e,  // ( INTEGER2 )
  //
  XY          =0x10,  // ( INTEGER4 )
  ENDEL       =0x11,  // ( NoData )
  SNAME       =0x12,  // ( String )
  //
  //
  //
  TEXTTYPE    =0x16,  // ( Integer2 )
  PRESENTATION=0x17,  // ( BitArray )
  //
  STRING      =0x19,  // ( String )
  STRANS      =0x1a,  // ( BitArray )
  MAG         =0x1b,  // ( Real8 )
  //

  //
  RECORD_UNKNOWN     =0x79,
};

//
//  GDS::Data_type
//
enum Data_type : Byte
{
  NoData    =0x00,
  BitArray  =0x01,
  Integer_2 =0x02,
  Integer_4 =0x03,
  Real_4    =0x04,
  Real_8    =0x05,
  String    =0x06,
  DATA_UNKNOWN =0x79,
};

//
//  GDS::DATA
// 
class DATA
{
 private:
  // RecordHeader //
  Byte                  RecordHeader_size1_;  // 0xXX
  Byte                  RecordHeader_size2_;  // 0xXX
  Record_type           RecordHeader_name_;   // 0xXX
  Data_type             RecordHeader_type_;   // 0xXX

  // Data //
  std::vector<Byte>     Data_list_;     //

 public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  DATA( )=default;
  ~DATA()=default;

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  DATA(const DATA&)=default;
  DATA& operator=(const DATA&)=default;

   /* ------------------ *
   *|  Move Construct  |*
   * ------------------ */
  DATA(DATA&&)=default;
  DATA& operator=(DATA&&)=default;

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  DATA( Byte name, Byte type ):
    RecordHeader_size1_(),
    RecordHeader_size2_(),
    RecordHeader_name_(static_cast<Record_type>(name)),
    RecordHeader_type_(static_cast<Data_type>(type))
  {}

  DATA( Record_type name, Data_type type, const std::string word ):
    RecordHeader_size1_(),
    RecordHeader_size2_(),
    RecordHeader_name_(name),
    RecordHeader_type_(type)
  { put_data_String(word); resize(); }

  DATA( std::ifstream& file ):
    RecordHeader_size1_(),
    RecordHeader_size2_(),
    RecordHeader_name_(),
    RecordHeader_type_()
  { read_gdsii(file); }

  /*
  DATA( const std::vector<Byte>& data_list ):
    RecordHeader_size1_(),
    RecordHeader_size2_(),
    RecordHeader_name_(),
    RecordHeader_type_()
  { Data_list_ = data_list; }
  */

  /* ------- *
   *|  put  |*
   * ------- */
  void put_size( int );
  void put_name( Record_type );
  void put_type( Data_type );
  void put_data_Integer2( short );
  void put_data_Integer4( int );
  void put_data_Real4( double );
  void put_data_Real8( double );
  void put_data_String( const std::string& );

  void put_size( Byte, Byte );
  void put_name( Byte );
  void put_type( Byte );
  void put_data( Byte );

  /* ------- *
   *|  get  |*
   * ------- */
  Record_type get_record( )const{return RecordHeader_name_;}
  short       get_size( )const;
  std::string get_name( )const;
  std::string get_type( )const;
  std::vector<short>  get_data_Integer2( )const;
  std::vector<int>    get_data_Integer4( )const;
  std::vector<double> get_data_Real8( )const;
  std::string         get_data_String( )const;

  /* --------- *
   *|  other  |*
   * --------- */
  short resize();

  /* -------- *
   *|  show  |*
   * -------- */
  void show_size( )const;
  void show_name( )const;
  void show_type( )const;
  void show_data( )const;
  void show_data_Integer2( )const;
  void show_data_Integer4( )const;
  void show_data_Real8( )const;
  void show_data_String( )const;
  void show_data_Other( )const;

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  void read_gdsii( std::ifstream& );

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( std::ofstream& )const;

  
  /* -------- *
   *|  make  |*
   * -------- */
  /*
  // size //
  int  make_size();
  int  make_size( int );
  // data(standard) //
  void make_data_Byte2( short );
  void make_data_Byte4( int );
  void make_data_String( string );
  // data(special) //
  void make_data_Header_5( );
  void make_data_Header_6( );
  void make_data_CurrentTime();
  void make_data_Units();
  void make_data_Angle( const string& );
  void make_data_CenterMiddle();
  */
};

//
//  GDS::EL
//
class EL
{
 private:
  DATA                EL_bgn_;    // Record_type == BOUNDARY,PATH,TEXT,SREF,AREF
  std::vector<DATA>   EL_list_;   //
  DATA                EL_end_;    // Record_type == ENDEL 
  //
  //  Records of Entities Within of Structure
  //    BOUNDARY [LAYER,DATATYPE]
  //    PATH [LAYER,DATATYPE,PATHTYPE,WIDTH]
  //    TEXT[LAYER,DATATYPE,TEXTTYPE,PRESENTATION,STRING]
  //  Records of References Inside of a Structure
  //    SREF [STRANS,MAG,ANGLE]
  //    AREF [STRANS,MAG,ANGLE.COLROW]
  //

 public:
    /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  EL()=default;           // Default Construct
  ~EL()=default;          // Destruct

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  EL(const EL&)=default;
  EL& operator=(const EL&)=default;

  /* ------------------ *
   *|  Move Construct  |*
   * ------------------ */
  EL(EL&&)=default;
  EL& operator=(EL&&)=default;

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  EL( DATA&& gds ):
    EL_bgn_( gds )
  {}

  /* ------- *
   *|  put  |*
   * ------- */
  void put_ENDEL( DATA&& );
  void put_DATA( DATA&& );
  void put_DATA( std::vector<DATA>&& );

  /* ------- *
   *|  get  |*
   * ------- */
  std::string get_type( )const;

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  //void read_gdsii( std::ifstream& );

  /* --------- *
   *|  parse  |*
   * --------- */
  void parse_gdsii( DATA&& );

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( std::ofstream& )const;
};

//
//  GDS::STR
//
class STR
{
 private:
  DATA                 STR_bgn_;     // Record_type == BGNSTR
  DATA                 STR_name_;    // Record_type == STRNAME
  std::vector<EL>      STR_list_;    //
  DATA                 STR_end_;     // Record_type == ENDSTR
  //

 public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  STR()=default;           // Default Constructor
  ~STR()=default;          // Destructor

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  STR(const STR&)=default;             // Copy Constructor       
  STR& operator=(const STR&)=default;  // Copy Constructor

  /* ------------------ *
   *|  Move Construct  |*
   * ------------------ */
  STR(STR&&)=default;                  // Move Constructor
  STR& operator=(STR&&)=default;       // Move Constructor

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  STR( DATA&& gds ):
    STR_bgn_( gds )
  {}

  /* ------- *
   *|  put  |*
   * ------- */
  void put_name( const std::string& );
  void put_EL( std::vector<EL>&& );

  /* ------- *
   *|  get  |*
   * ------- */
  std::string get_name( )const;

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* --------- *
   *|  parse  |*
   * --------- */
  void parse_gdsii( DATA&& ); 

  /* -------- *
   *|  Read  |*
   * -------- */
  //void read_gdsii( std::ifstream& );

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( std::ofstream& )const;

  /* -------- *
   *|  make  |*
   * -------- */
  /*
  void make_BOUNDARY(short,short,int,int,int,int);
  void make_SREF(const string&,const string&,int,int);
  void make_TEXT(const string&,short,short,const string&,int,int);
  */
};

class LIB
{
private:
  DATA                                       LIB_hdr_;
  DATA                                       LIB_bgn_;
  DATA                                       LIB_name_;
  DATA                                       LIB_units_;

  std::vector<STR>                           LIB_list_; 

  DATA                                       LIB_end_;

public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  LIB()=default;                 // Default Constructor
  ~LIB()=default;                // Destructor

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  LIB(const LIB&)=default;            // Copy Constructor
  LIB& operator=(const LIB&)=default; // Copy Constructor

  /* ------------------ *
   *|  Move Construct  |*
   * ------------------ */
  LIB(LIB&&)=default;            // Move Constructor
  LIB& operator=(LIB&&)=default; // Move Constructor

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  LIB(const char* f_name)
  {
    read_gdsii( std::string(f_name) );
  }
  LIB(const std::string& f_name)
  {
    read_gdsii( f_name );
  }

  /* ------- *
   *|  put  |*
   * ------- */
  void put_version( short );
  void put_name( const std::string& );
  void put_STR( STR&& );
  void put_STR( std::vector<STR>&& );

  /* ------- *
   *|  get  |*
   * ------- */
  short        get_version( )const;
  std::string  get_name( )const;

  /* ------ *
   *|  at  |*
   * ------ */
  EL& operator[]( const std::string& );

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  void read_gdsii( const std::string& f_name){
    std::ifstream file(f_name);
    read_gdsii(file);
  }
  void read_gdsii( std::ifstream& );
 
  /* --------- *
   *|  parse  |*
   * --------- */
  void parse_gdsii( DATA&& ); 

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( std::ofstream& )const;
   
};



/* ----------- *
 *|  product  |*
 * ----------- */
DATA product_HEADER( short );
DATA product_NAME( Record_type, const std::string& );
DATA product_STRNAME( const std::string& );
DATA product_LIBNAME( const std::string& );

/*
// 0x00 //
DATA product_NoData( Byte );                  // 0x00
// 0x01 //
DATA product_STRANS( Byte, const string& );   // 0x01
DATA product_CenterMiddle( Byte );            // 0x01
// 0x02 //
DATA product_Integer2( Byte, short );
DATA product_Integer2( Byte, const vector<short>& );
DATA product_Integer2_Coord( Byte, short, short );
// 0x03 //
DATA product_Integer4( Byte, const vector<int>& );
DATA product_Integer4_Coord( Byte, int, int );
DATA product_Integer4_Rect( Byte, int, int, int, int );
DATA product_Integer4_Array( Byte, int, int, int, int );
// 0x06 //
DATA product_String( Byte, const string& );   // 0x06 
// BGN,END,NAME //
DATA product_BGN( Byte );
DATA product_NAME( Byte, const string& );
DATA product_END( Byte );
// STR
DATA product_STRBGN( );
DATA product_STRNAME( const string& );
DATA product_STREND( );
// LIB //
DATA product_HEADER( );
DATA product_LIBBGN( );
DATA product_LIBNAME( const string& );
DATA product_UNITS( );
DATA product_LIBEND( );
*/

/* ------------------- *
 *|  Append (static)  |*
 * ------------------- */
/*
void append_BOUNDARY( vector<DATA>&, short, short, int, int, int, int );
void append_SREF( vector<DATA>&, const string&, const string&, int, int );
void append_AREF( vector<DATA>&, const string&, const string&, int, int, short, short, int, int );
void append_TEXT( vector<DATA>&, const string&, short, short, const string&, int, int );
*/

}


#endif

