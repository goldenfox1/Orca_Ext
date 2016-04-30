{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2TextControl =============================

constructor TD2TextControl.Create(AOwner:TComponent);
begin
  inherited;
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  FFontFill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FFontFill.OnChanged:=FontFillChanged;
  FWordWrap:=false;
end;

destructor TD2TextControl.Destroy;
begin
  FFontFill.Free;
  FFont.Free;
  inherited;
end;

function TD2TextControl.GetData:Variant;
begin
  Result:=TextW;
end;

procedure TD2TextControl.SetData(const Value:Variant);
begin
  if VarIsNull(Value) then
    TextW:=''
  else
    if VarIsType(Value,varDate) then
      TextW:=DateTimeToStr(VarToDateTime(Value))
    else
      TextW:=VarToWideStr(Value);
end;

procedure TD2TextControl.ApplyStyle;
var
  S:TD2Object;
  NewT:WideString;
begin
  inherited;
  { from style }
  S:=FindResource('foreground');
  if (S <> nil) and (S is TD2BrushObject) then
    FontFill.Assign(TD2BrushObject(S).Brush);
  { to style }
  S:=FindResource('text');
  if (S <> nil) and (S is TD2Text) then
  begin
    TD2Text(S).TextW:=FText;
    TD2Text(S).HorzTextAlign:=FTextAlign;
    TD2Text(S).VertTextAlign:=FVertTextAlign;
    TD2Text(S).WordWrap:=FWordWrap;
    TD2Text(S).Font.Assign(FFont);
  end;
  { translate }
  if FAutoTranslate then
  begin
    NewT:=Translate(TextW); // need for collection texts
    if (FScene <> nil) and not (FScene.GetDesignTime) then
      TextW:=NewT;
  end;
end;

procedure TD2TextControl.FontChanged(Sender:TObject);
var
  T:TD2Object;
begin
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
    TD2Text(T).Font.Assign(FFont);
end;

procedure TD2TextControl.FontFillChanged(Sender:TObject);
var
  T:TD2Object;
begin
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
    TD2Text(T).Fill.Assign(FontFill);
end;

procedure TD2TextControl.SetText(const Value:WideString);
var
  T:TD2Object;
begin
  if FText <> Value then
  begin
    FText:=Value;
    T:=FindResource('text');
    if (T <> nil) and (T is TD2Text) then
    begin
      TD2Text(T).TextW:=FText;
      TD2Text(T).UpdateEffects;
    end
    else
      if (FResourceLink <> nil) and (FResourceLink is TD2Text) then
        TD2Text(FResourceLink).TextW:=FText
      else
        Repaint;
    UpdateEffects;
  end;
end;

procedure TD2TextControl.SetText(const Value:String);
  var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  SetText(ss);
end;

function TD2TextControl.GetText:WideString;
begin
  Result:=Ftext;
end;

function TD2TextControl.GetText:String;
begin
  Result:=UTF8Encode(Ftext);
end;

procedure TD2TextControl.SetFontFill(const Value:TD2Brush);
begin
  FFontFill:=Value;
end;

procedure TD2TextControl.SetFont(const Value:TD2Font);
begin
  FFont.Assign(Value);
end;

procedure TD2TextControl.SetTextAlign(const Value:TD2TextAlign);
var
  T:TD2Object;
begin
  FTextAlign:=Value;
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
    TD2Text(T).HorzTextAlign:=FTextAlign
  else
    Repaint;
end;

procedure TD2TextControl.SetVertTextAlign(const Value:TD2TextAlign);
var
  T:TD2Object;
begin
  FVertTextAlign:=Value;
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
    TD2Text(T).VertTextAlign:=FVertTextAlign
  else
    Repaint;
end;

procedure TD2TextControl.SetWordWrap(const Value:boolean);
var
  T:TD2Object;
begin
  FWordWrap:=Value;
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
    TD2Text(T).WordWrap:=Value;
end;

//=============== TD2Label =========================================

constructor TD2Label.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Width:=120;
  Height:=15;
  FWordWrap:=true;
  HitTest:=false;
end;

procedure TD2Label.ApplyStyle;
var
  T:TD2Object;
  S:TD2Align;
begin
  inherited;
  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
  begin
    TD2Text(T).WordWrap:=WordWrap;
    if AutoSize then
    begin
      FWordWrap:=false;
      TD2Text(T).WordWrap:=false;
      TD2Text(T).VertTextAlign:=d2TextAlignNear;
      TD2Text(T).HorzTextAlign:=d2TextAlignNear;

      S:=TD2Text(T).Align;
      TD2Text(T).Align:=vaNone;
      TD2Text(T).AutoSize:=true;
      Width:=TD2Text(T).Width;
      Height:=TD2Text(T).Height;
      TD2Text(T).AutoSize:=false;
      TD2Text(T).Align:=S;
    end;
  end;
end;

procedure TD2Label.SetWordWrap(const Value:boolean);
var
  T:TD2Object;
begin
  if FWordWrap <> Value then
  begin
    FWordWrap:=Value;
    T:=FindResource('text');
    if (T <> nil) and (T is TD2Text) then
      TD2Text(T).WordWrap:=Value;
  end;
end;

procedure TD2Label.SetAutoSize(const Value:boolean);
begin
  if FAutoSize <> Value then
  begin
    FAutoSize:=Value;
    if FAutoSize then
    begin
      ApplyStyle;
    end;
  end;
end;

procedure TD2Label.SetText(const Value:String); // 7777
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);

  if FText <> ss then                 // *** DB ***
  begin
    inherited;
    if FAutoSize then
      ApplyStyle;
  end
  else
    inherited;
end;

//=============== TD2ValueLabel ==============================

constructor TD2ValueLabel.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=false;
  FWordWrap:=false;
end;
               
             
                                 
