program MultMatrix;

uses Dos, sysutils;

var
        matrix1: array of array of real;
        matrix2: array of array of real;
        matrixresultado: array of array of real;
        i: integer;
        j: integer;
        k: integer;
        lin, col: integer;
        temp: real;
		hours: word;
		minutes: word;
		seconds: word;
		milliseconds: word;


procedure StartClock;
begin
  GetTime(hours, minutes, seconds, milliseconds);
end;

procedure StopClock;
var seconds_count : longint;
    miliseconds_count : longint;
    c_hours: word;
    c_minutes: word;
    c_seconds: word;
    c_milliseconds: word;

begin
  GetTime(c_hours, c_minutes, c_seconds, c_milliseconds);
  seconds_count := c_seconds - seconds + (c_minutes - minutes) * 60 + (c_hours - hours) * 3600;
  miliseconds_count := c_milliseconds-milliseconds;
  writeln(inttostr(seconds_count) + ' seconds' + inttostr(miliseconds_count)+' hsec');
end;



begin
        write('Numero de Linhas');
        readln(lin);
        SetLength(matrix1,lin+1,lin+1);
        SetLength(matrix2,lin+1,lin+1);
        SetLength(matrixresultado,lin+1,lin+1);
        FOR i:=1 to lin DO
        begin
                FOR j:=1 to lin DO
                begin
                        matrix1[i][j]:=1;
                        matrix2[i][j]:=i;
                end;
        end;

        writeln('1 - Versao Standart');
        writeln('2 - Versao Modificada');
        readln(i);

        StartClock;
        if i = 1
        then begin

                FOR i:=1 to lin DO
                begin
                        FOR j:=1 to lin DO
                        begin
                                temp:=0;
                                FOR k:=1 to lin DO
                                begin
                                        temp := temp + matrix1[i][k] * matrix2[k][j]
                                end;
                                matrixresultado[i][j]:=temp;
                         end;
                end;
        end

        else    begin

                FOR i:=1 to lin DO
                begin
                        FOR k:=1 to lin DO
                        begin
                                FOR j:=1 to lin DO
                                begin
                                        matrixresultado[i][j] := matrixresultado[i][j]+matrix1[i][k]*matrix2[k][j];
                                end;
                        end;
                end;


        end;

        StopClock;

        readln();
        writeln('Fim');
end.

