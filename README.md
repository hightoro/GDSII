GDSII
=====
LSIレイアウトの標準ストリームフォーマットGDSIIの操作用オレオレクラス  
GDSIIのバージョン5.0の最低限の表現までしかサポートされていません  
  
 * クラス構造  
　LIBRARY(STRUCTUREの集まり)  
   -STRUCTURE(ELの集合。これが1つのレイアウト)  
     -EL(レクトアングル、パス、STRUCTURE参照などの要素)  
       -Record(Header(4byte)+Data(0byte~))  
          -Byte_data (8bit)  

