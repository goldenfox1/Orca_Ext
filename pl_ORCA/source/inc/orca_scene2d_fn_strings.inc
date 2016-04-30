{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


// Geom ========================================================================

function d2GetToken(var S:Ansistring; Separators:Ansistring; Stop:Ansistring=''):Ansistring;
var
  i,Len:integer;
  CopyS:Ansistring;
begin
  Result:='';
  CopyS:=S;
  Len:=Length(CopyS);
  for i:=1 to Len do
  begin
    if Pos(CopyS[i],Stop) > 0 then
      Break;
    Delete(S,1,1);
    if Pos(CopyS[i],Separators) > 0 then
    begin
      Result:=Result;
      Break;
    end;
    Result:=Result + CopyS[i];
  end;
  Result:=Trim(Result);
  S:=Trim(S);
end;

function d2WideGetToken(var Pos:integer; const S:WideString; Separators:WideString; Stop:WideString=''):WideString;
var
  i,Len:integer;
begin
  Result:='';
  Len:=Length(S);
  { skip first separators }
  for i:=Pos to Len do
    if System.Pos(S[i],Separators) <= 0 then
      Break;
  Pos:=i;
  { get }
  for i:=Pos to Len do
  begin
    if System.Pos(S[i],Stop) > 0 then
      Break;
    if System.Pos(S[i],Separators) > 0 then
      Break;
    Result:=Result + S[i];
  end;
  { skip separators }
  Pos:=i;
  for i:=Pos to Len do
    if System.Pos(S[i],Separators) <= 0 then
      Break;
  Pos:=i;
end;

procedure d2ReverseBytes(p:Pointer; Count:integer);
var
  p1:PChar;
  p2:PChar;
  c:Char;
begin
  p1:=PChar(p);
  p2:=PChar(p)+Count-1;
  while p1<p2 do begin
    c:=p1^;
    p1^:=p2^;
    p2^:=c;
    System.inc(p1);
    System.dec(p2);
  end;
end;

procedure d2MoveLongword(const Src:Pointer; Dst:Pointer; Count:integer);
begin
  System.Move(Src^,Dst^,Count * 4); // move() is optimized and fast
end;

procedure d2FillLongword(Src:Pointer; Count:integer; Value:Longword);
var
  i:integer;
begin
  {$ifdef cpu64}
  for i:=0 to Count - 1 do
    PD2ColorArray(Src)[i]:=Value;
  {$else}
  for i:=1 to Count shr 2 do
  begin // 4 DWORD by loop - aligned pipelined write
    PD2ColorArray(Src)[0]:=Value;
    PD2ColorArray(Src)[1]:=Value;
    PD2ColorArray(Src)[2]:=Value;
    PD2ColorArray(Src)[3]:=Value;
    inc(cardinal(Src),16);
  end;
  for i:=0 to (Count and 3)-1 do // last 0..3 DWORD
    PD2ColorArray(Src)[i]:=Value;
  {$endif}
end;

procedure d2FillAlpha(Src:Pointer; Count:integer; Alpha:byte);
var
  i:integer;
begin
  for i:=0 to Count - 1 do
    PD2ColorRecArray(Src)[i].A:=Alpha;
end;

procedure d2FillLongwordRect(Src:Pointer; W,H,X1,Y1,X2,Y2:integer; Value:Longword);
var
  j:integer;
begin
  if x2 > W then x2:=W;
  if y2 > H then y2:=H;
  if x1 > x2 then x1:=x1;
  if y1 > y2 then y1:=y2;
  for j:=y1 to y2 - 1 do
    d2FillLongword(@PD2ColorArray(Src)[X1 + (j * W)],X2 - X1,Value);
end;



