#ifndef CLASS_GDS_H
#define CLASS_GDS_H
/*+--------------------------- *
 *|                           |*
 *|  File : GDS.hpp           |*
 *|                           |*
 *|  Data : 06/12/2011        |*
 *|  Type : Class Header      |*
 *|  Base : Nothing           |*
 *|                           |*
 * --------------------------- */
namespace GDS {

  class LIB;   // Library
  class STR;   // Structure ( some of LE )
  class EL;    // Element ( some of Records )
  class DATA;  // Records ( minimam 4 bytes )
//class Byte;  // bytes( 8bit data )


//
//  GDS::Record_type
// 
enum Record_type : unsigned char  // ( Data_type )
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
  RECORD_UNKNOWN     =0x99,
};

//
//  GDS::Data_type
// 
enum Data_type : unsigned char
{
  NoData    =0x00,
  BitArray  =0x01,
  Integer_2 =0x02,
  Integer_4 =0x03,
  Real_4    =0x04,
  Real_8    =0x05,
  String    =0x06,
  DATA_UNKNOWN =0x99,
};

//
//  GDS::Byte
// 
typedef char Byte;
typedef basic_ifstream<Byte> byte_ifstream;
//
//  GDS::DATA
// 
class DATA
{
 private:
  // RecordHeader //
  Byte      RecordHeader_size1;  // 0xXX
  Byte      RecordHeader_size2;  // 0xXX
  Byte      RecordHeader_name;   // 0xXX // Record_type
  Byte      RecordHeader_type;   // 0xXX // Data_type

  // Data //
  vector<Byte>    Data_list;     //

 public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  DATA( ):
    RecordHeader_size1(0),
    RecordHeader_size2(0),
    RecordHeader_name(0),
    RecordHeader_type(0)
  {}
  ~DATA()=default;

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  DATA( const DATA& cp )=default;
  DATA& operator=( const DATA& cp )=default;
 
  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  DATA( Byte name, Byte type ):
    RecordHeader_size1(0),
    RecordHeader_size2(0),
    RecordHeader_name(name),
    RecordHeader_type(type)
  {}
  DATA( byte_ifstream& file ):
    RecordHeader_size1(0),
    RecordHeader_size2(0),
    RecordHeader_name(0),
    RecordHeader_type(0)
  { read_gdsii(file); }
  /*
  DATA( const vector<Byte>& data_list ):
    RecordHeader_size1(0),
    RecordHeader_size2(0),
    RecordHeader_name(0),
    RecordHeader_type(0)
  { Data_list = data_list; }
  */

  /* ----------------------- *
   *|  Init & Copy & Clear  |*
   * ----------------------- */
  void init( );
  void copy( const DATA& );
  void clear( );

  /* -------- *
   *|  make  |*
   * -------- */
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

  /* ------- *
   *|  put  |*
   * ------- */
  void put_size( Byte, Byte );
  void put_name( Byte );
  void put_type( Byte );
  void put_data( Byte );

  /* ------- *
   *|  get  |*
   * ------- */
  short  get_size( )const;
  string get_name( )const;
  string get_type( )const;
  short  get_data_Header( )const;
  string get_data_String( )const;

  /* -------- *
   *|  show  |*
   * -------- */
  void show_size( )const;
  void show_name( )const;
  void show_type( )const;
  void show_data( )const;
  void show_data_Integer2( )const;
  void show_data_Integer4( )const;
  void show_data_String( )const;
  void show_data_Other( )const;

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  void read_gdsii( byte_ifstream& );

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( ofstream& )const;

};

/* ----------- *
 *|  product  |*
 * ----------- */
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

//
//  GDS::STR
// 
class STR
{
 private:
  DATA                 STR_bgn;
  DATA                 STR_name;

  vector<DATA>         GDS_stream; 

  DATA                 STR_end;

 public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  STR(){                   // Default Construct
    STR_bgn=product_STRBGN();
    STR_end=product_STREND();
  }
  ~STR()=default;          // Destruct

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  STR( const STR& cp )=default;           
  STR& operator=( const STR& cp )=default;  

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  STR( const string& name )
  {
    STR_bgn  = product_STRBGN();
    STR_name = product_STRNAME( name );
    STR_end  = product_STREND();
  }
  STR( const DATA& gds ):STR_bgn( gds ){}

  /* ----------------------- *
   *|  Init & Copy & Clear  |*
   * ----------------------- */ 
  void init( );
  void copy( const STR& );
  void clear( );

  /* -------- *
   *|  make  |*
   * -------- */
  void make_BOUNDARY(short,short,int,int,int,int);
  void make_SREF(const string&,const string&,int,int);
  void make_TEXT(const string&,short,short,const string&,int,int);

  /* ------- *
   *|  put  |*
   * ------- */
  void put_name( const string& );
  void put_stream( const vector<DATA>& );

  /* ------- *
   *|  get  |*
   * ------- */
  string       get_name( )const;
  vector<DATA> get_stream( )const;

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  void read_gdsii( byte_ifstream& );

  /* --------- *
   *|  parse  |*
   * --------- */
  void parse_gdsii( const DATA& ); 

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( ofstream& )const;

};


class LIB
{
private:
  //int                                            GDS_ver;
  //string                                         LIB_name;

  DATA                                       LIB_hdr;
  DATA                                       LIB_bgn;
  DATA                                       LIB_name;
  DATA                                       LIB_units;

  vector<STR>                                STR_stream; 

  DATA                                       LIB_end;

public:
  /* ------------------------ *
   *|  Construct & Destruct  |*
   * ------------------------ */
  LIB()
  {
    LIB_hdr   = product_HEADER();
    LIB_bgn   = product_LIBBGN();
    LIB_units = product_UNITS();
    LIB_end   = product_LIBEND();
  }                              // Default Construct
  ~LIB()=default;                // Destruct

  /* ------------------ *
   *|  Copy Construct  |*
   * ------------------ */
  LIB( const LIB& cp )  =default;          // Copy Construct
  LIB& operator=( const LIB& cp )=default; // Copy Construc

  /* ------------------- *
   *|  Other Construct  |*
   * ------------------- */
  LIB( const char* f_name )
  {
    read_gdsii((string)f_name);
  }
  LIB( const string& f_name )
  {
    read_gdsii(f_name);
  }

  /* ----------------------- *
   *|  Init & Copy & Clear  |*
   * ----------------------- */ 
  void init( );
  void copy( const LIB& );
  void clear( );

  /* ------- *
   *|  put  |*
   * ------- */
  void put_ver( const short& );
  void put_name( const string& );
  void put_stream( const STR& );
  void put_stream( const vector<STR>& );

  /* ------- *
   *|  get  |*
   * ------- */
  short        get_ver( );
  string       get_name( );
  vector<STR>  get_stream( );

  /* --------- *
   *|  Print  |*
   * --------- */
  void print( )const;

  /* -------- *
   *|  Read  |*
   * -------- */
  void read_gdsii( const string& );
  void read_gdsii( byte_ifstream& );
  byte_ifstream& file_open( byte_ifstream& file, const string& f_name );  

  /* --------- *
   *|  parse  |*
   * --------- */
  void parse_gdsii( const DATA& ); 

  /* --------- *
   *|  Write  |*
   * --------- */
  void write_gdsii( ofstream& );
   
};

/* ------------------- *
 *|  Append (static)  |*
 * ------------------- */
void append_BOUNDARY( vector<DATA>&, short, short, int, int, int, int );
void append_SREF( vector<DATA>&, const string&, const string&, int, int );
void append_AREF( vector<DATA>&, const string&, const string&, int, int, short, short, int, int );
void append_TEXT( vector<DATA>&, const string&, short, short, const string&, int, int );

}

#endif

