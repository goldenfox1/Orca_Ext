{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

function d2RGBtoBGR(const C:TD2Color):TD2Color;
begin
  Result:=C;
  TD2ColorRec(Result).R:=TD2ColorRec(C).B;
  TD2ColorRec(Result).B:=TD2ColorRec(C).R;
end;

function d2ColorFromVCL(const C:TColor):TD2Color;
begin
  Result:=d2RGBtoBGR(ColorToRGB(C)) or $FF000000;
end;

function d2CorrectColor(const C:TD2Color):TD2Color;
begin
  Result:=C;
  {$IFDEF DARWIN}
  TD2ColorRec(Result).R:=TD2ColorRec(C).B;
  TD2ColorRec(Result).B:=TD2ColorRec(C).R;
  {$ENDIF}
end;

function d2PremultyAlpha(const C:TD2Color):TD2Color;
begin
  if TD2ColorRec(C).A=0 then
    Result:=0
  else
  if TD2ColorRec(C).A=$FF then
    Result:=C
  else
  begin
    TD2ColorRec(Result).R:=trunc(TD2ColorRec(C).R * (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).G:=trunc(TD2ColorRec(C).G * (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).B:=trunc(TD2ColorRec(C).B * (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).A:=TD2ColorRec(C).A;
  end;
end;

function d2UnpremultyAlpha(const C:TD2Color):TD2Color;
begin
  if TD2ColorRec(C).A=0 then
    Result:=0
  else
  if TD2ColorRec(C).A=$FF then
    Result:=C
  else
  begin
    TD2ColorRec(Result).R:=trunc(TD2ColorRec(C).R / (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).G:=trunc(TD2ColorRec(C).G / (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).B:=trunc(TD2ColorRec(C).B / (TD2ColorRec(C).A / $FF));
    TD2ColorRec(Result).A:=TD2ColorRec(C).A;
  end;
end;

function d2Opacity(const C:TD2Color; const AOpacity:single):TD2Color;
begin
  Result:=C;
  if AOpacity < 1 then
    TD2ColorRec(Result).A:=Trunc(TD2ColorRec(C).A * AOpacity);
end;

function d2Color(R,G,B:Byte; A:Byte=$FF):TD2Color;
begin
  TD2ColorRec(Result).R:=R;
  TD2ColorRec(Result).G:=G;
  TD2ColorRec(Result).B:=B;
  TD2ColorRec(Result).A:=A;
end;

function d2ChangeHSL(const C:TD2Color; dH,dS,dL:single):TD2Color;
var
  H,S,L:single;
  A:byte;
begin
  A:=TD2ColorRec(c).A;
  d2RGBtoHSL(C,H,S,L);
  H:=H + dH;
  if H < 0 then H:=0;
  if H > 1 then H:=1;
  S:=S + dS;
  if S < 0 then S:=0;
  if S > 1 then S:=1;
  L:=L + dL;
  if L < 0 then L:=0;
  if L > 1 then L:=1;
  Result:=d2HSLtoRGB(H,S,L);
  TD2ColorRec(Result).A:=A;
end;

function d2ColorToStr(Value:TD2Color):string;
begin
  Result:='#' + IntToHex(Value,8);
end;

function d2StrToColor(Value:string):TD2Color;
var
  i:integer;
begin
  if Value=#0 then
    Value:='$0'
  else
    if (Value <> '') and (Value[1]='#') then
      Value[1]:='$'
    else
    begin
      for i:=0 to High(d2ColorIdents) do
        if CompareText(Value,d2ColorIdents[i].Name)=0 then
        begin
          Value:=d2ColorIdents[i].Value;
          Value[1]:='$';
          Break;
        end;
    end;
  try
    Result:=StrToInt(Value);
  except
  end;
end;

function d2FloatToStr(Value:single):string;
var
  S:char;
begin
  S:=DecimalSeparator;
  try
    DecimalSeparator:='.';
    if Frac(Value) <> 0 then
      Result:=Format('%.3f',[Value])
    else
      Result:=IntToStr(Trunc(Value))
  finally
    DecimalSeparator:=S;
  end;
end;

function d2StrToFloat(Value:string):single;
var
  S:char;
begin
  S:=DecimalSeparator;
  try
    DecimalSeparator:='.';
    Result:=StrToFloat(Value);
  finally
    DecimalSeparator:=S;
  end;
end;

function d2HSLtoRGB(H,S,L:single):TD2Color;
  function hue2rgb(p,q,t:single):single;
  begin
    if (t < 0) then t:=t + 1;
    if (t > 1) then t:=t - 1;
    if (t < 1/6) then
    begin
      Result:=p + (q - p) * 6 * t;
      Exit;
    end;
    if (t < 1/2) then
    begin
      Result:=q;
      Exit;
    end;
    if (t < 2/3) then
    begin
      Result:=p + (q - p) * (2/3 - t) * 6;
      Exit;
    end;
    Result:=p;
  end;
var
  r,g,b:single;
  q,p:single;
begin
  if (s=0) then
  begin
    r:=L;
    g:=L;
    b:=L;
  end
  else
  begin
    if (L < 0.5) then
      q:=L * (1 + S)
    else
      q:=L + S - L * S;
    p:=2 * L - q;
    r:=hue2rgb(p,q,H + 1/3);
    g:=hue2rgb(p,q,H);
    b:=hue2rgb(p,q,H - 1/3);
  end;
  Result:=d2Color(round(r * $FF),round(g * $FF),round(b * $FF));
end;

procedure d2RGBtoHSL(RGB:TD2Color; out H,S,L:single);
var
  r,g,b:single;
  d,mx,mn:single;
begin
  r:=TD2ColorRec(RGB).R / $FF;
  g:=TD2ColorRec(RGB).G / $FF;
  b:=TD2ColorRec(RGB).B / $FF;
  mx:=Max(Max(R,G),B);
  mn:=Min(Min(R,G),B);
  H:=(mx + mn) / 2;
  L:=H;
  S:=H;
  if (mx=mn) then
  begin
    S:=0;
    H:=0;
  end
  else
  begin
    d:=mx - mn;
    if L > 0.5 then
      S:=d / (2 - mx - mn)
    else
      S:=d / (mx + mn);
    if (mx=r) then
      H:=(g - b) / d
    else
    if (mx=g) then
      H:=(b - r) / d + 2
    else
      H:=(r - g) / d + 4;
    H:=H / 6;
    if H < 0 then H:=H + 1;
  end;
end;
      
