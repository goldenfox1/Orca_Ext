{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2StorageItem ==============================================================}

constructor TD2StorageItem.Create;
begin
  inherited Create;
end;

destructor TD2StorageItem.Destroy;
begin
  if FStream <> nil then
    FStream.Free;
  inherited;
end;

procedure TD2StorageItem.ReadItem(R:TReader);
var
  Count:Longint;
begin
  FType:=R.NextValue;
  case FType of
    vaWString:FAsString:=R.ReadWideString;
    vaString,vaLString:FAsString:=R.ReadString;
    vaSingle:FAsFloat:=R.ReadSingle;
    vaExtended:FAsFloat:=R.ReadFloat;
    vaInt8,vaInt16,vaInt32:FAsInteger:=R.ReadInteger;
    vaBinary:
      begin
        R.ReadValue;
        R.Read(Count,SizeOf(Count));
        AsStream.SetSize(Count);
        R.Read(AsStream.Memory^,Count);
      end;
    vaTrue,vaFalse:R.ReadBoolean;
  end;
end;

procedure TD2StorageItem.WriteItem(W:TWriter);
var
  Count:Longint;
begin
  case FType of
    vaWString:W.WriteWideString(FAsString);
    vaString,vaLString:W.WriteString(FAsString);
    vaSingle,vaExtended:W.WriteFloat(FAsFloat);
    vaInt8,vaInt16,vaInt32:W.WriteInteger(FAsInteger);
    vaBinary:
      begin
        WriteValue(W,vaBinary);
        Count:=FStream.Size;
        W.Write(Count,SizeOf(Count));
        W.Write(FStream.Memory^,FStream.Size);
      end;
    vaTrue:WriteValue(W,vaTrue);
    vaFalse:WriteValue(W,vaFalse);
  else
    Count:=1;
  end;
end;

function TD2StorageItem.GetAsBool:boolean;
begin
  Result:=FType=vaTrue;
end;

function TD2StorageItem.GetAsStream:TMemoryStream;
begin
  FType:=vaBinary;
  if FStream=nil then
    FStream:=TMemoryStream.Create;
  Result:=FStream;
end;

procedure TD2StorageItem.SetAsFloat(const Value:single);
begin
  FType:=vaSingle;
  FAsFloat:=Value;
end;

procedure TD2StorageItem.SetAsString(const Value:WideString);
begin
  FType:=vaWString;
  FAsString:=Value;
end;

procedure TD2StorageItem.SetAsInteger(const Value:integer);
begin
  FType:=vaInt32;
  FAsInteger:=Value;
end;

procedure TD2StorageItem.SetAsBool(const Value:boolean);
begin
  if Value then
    FType:=vaTrue
  else
    FType:=vaFalse;
end;

//=============== TD2Storage =================================================

const
  FilerSignature:array[1..4] of Char='TPF0';

constructor TD2Storage.Create;
begin
  inherited;
  FItems:=TD2WideStringList.Create;
end;

destructor TD2Storage.Destroy;
begin
  FItems.Free;
  inherited;
end;

function TD2Storage.GetValues(Name:string):TD2StorageItem;
var
  Idx:integer;
  Item:TD2StorageItem;
begin
  Idx:=FItems.IndexOf(Name);
  if Idx >= 0 then
    Result:=TD2StorageItem(FItems.Objects[Idx])
  else
  begin
    Item:=TD2StorageItem.Create;
    FItems.AddObject(Name,Item);
    Result:=Item;
  end;
end;

procedure TD2Storage.SaveToStream(S:TStream);
var
  i:integer;
  W:TWriter;
begin
  W:=TWriter.Create(S,1024);
  try
    W.Write(FilerSignature,4);
    WriteStr(W,'Root');
    WriteStr(W,'');
    for i:=0 to FItems.Count - 1 do
    begin
      WriteStr(W,FItems[i]);
      TD2StorageItem(FItems.Objects[i]).WriteItem(W);
    end;
    W.WriteListEnd;
    // objects
    W.WriteListEnd;
  finally
    W.Free;
  end;
end;

procedure TD2Storage.LoadFromStream(S:TStream);
var
  R:TReader;
  Signature:Longint;
begin
  R:=TReader.Create(S,1024);
  try
    R.Read(Signature,SizeOf(Signature));
    ReadStr(R);
    ReadStr(R);
    while not R.EndOfList do
    begin
      Values[ReadStr(R)].ReadItem(R);
    end;
  finally
    R.Free;
  end;
end;

procedure TD2Storage.LoadFromFile(FileName:string);
var
  B,S:TStream;
begin
  S:=TFileStream.Create(FileName,fmOpenRead);
  try
    B:=TMemoryStream.Create;
    ObjectTextToBinary(S,B);
    B.Position:=0;
    LoadFromStream(B);
    B.Free;
  finally
    S.Free;
  end;
end;

procedure TD2Storage.SaveToFile(FileName:string);
var
  B,S:TStream;
begin
  S:=TFileStream.Create(FileName,fmCreate);
  try
    B:=TMemoryStream.Create;
    SaveToStream(B);
    B.Position:=0;
    ObjectBinaryToText(B,S);
    B.Free;
  finally
    S.Free;
  end;
end;
                            
