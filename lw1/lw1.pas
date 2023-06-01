PROGRAM SurnameFinder(INPUT, OUTPUT);

VAR
  CurSurname, FirstSurname: STRING;
  FileName: STRING;
  InFile: FILE OF CHAR;
  FirstByte, CurByte, I: INTEGER;
  Ch, Symb: CHAR;
  
PROCEDURE ErrorLog(Mess: STRING);
BEGIN
  WRITELN(Mess);
  READLN;
  Halt
END;

PROCEDURE Init();
BEGIN
  IF ParamCount < 1
  THEN
    ErrorLog('Не указан исходный файл');
  FileName := ParamStr(1);   
  ASSIGN(InFile, FileName);
  RESET(InFile);
  IF IoResult <> 0
  THEN
    ErrorLog('Ошибка открытия файла ' + FileName);
  FirstSurname := '';
  CurSurname := '';
  FirstByte := 0;
  CurByte := 0;
  Symb := '*';  
END;

PROCEDURE ReplaceSurnameToStars();
BEGIN
  FOR I := 1 TO (LENGTH(FirstSurname))
  DO
    BEGIN
      SEEK(InFile, FirstByte);
      WRITE(InFile, Symb);
      FirstByte := FirstByte + 1;
    END;
END;    

PROCEDURE GetStr(VAR Str: STRING);
BEGIN
  Str := '';
  READ(InFile, Ch); 
  WHILE (Ch <> CHAR(13)) AND (NOT EOF(InFile)) 
  DO
    BEGIN 
      Str := Str + Ch;
      READ(InFile, Ch)
    END;
  IF EOF(InFile)
  THEN
    Str := Str + Ch;
  READ(InFile, Ch);
END;

BEGIN
  Init();
  IF NOT EOF(InFile)
  THEN
    BEGIN
      GetStr(CurSurname);
      CurByte := CurByte + LENGTH(CurSurname) + 2;
      FirstSurname := CurSurname;
      WHILE NOT EOF(InFIle)
      DO
        BEGIN
          GetStr(CurSurname);
          IF CurSurname < FirstSurname
          THEN 
            BEGIN
              FirstSurname := CurSurname;
              FirstByte := CurByte
            END;
          CurByte := CurByte + LENGTH(CurSurname) + 2;
        END;
      ReplaceSurnameToStars();
    END;
  Close(InFile) 
END.
