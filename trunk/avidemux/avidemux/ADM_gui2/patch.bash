cat GUI_main2.cpp | sed 's/, 0,$/, (GdkModifierType) 0,/g' | sed 's/-2/-1/g' | sed 's/GtkAccelGroup \*/extern GtkAccelGroup */'> GUI_main2.p
cp GUI_main2.cpp GUI_main2.tmp
cp GUI_main2.p GUI_main2.cpp
