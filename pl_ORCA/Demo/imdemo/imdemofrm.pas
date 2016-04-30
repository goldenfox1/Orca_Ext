{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit imdemofrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmIM = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Layout;
    back: TD2Rectangle;
    Rectangle1: TD2Rectangle;
    title: TD2Layout;
    Path1: TD2Path;
    Rectangle2: TD2Rectangle;
    SizeGrip1: TD2SizeGrip;
    avatar: TD2Rectangle;
    Image1: TD2Image;
    ShadowEffect1: TD2ShadowEffect;
    Text1: TD2Text;
    statusBox: TD2PopupBox;
    backShadow: TD2ShadowEffect;
    Text2: TD2Text;
    SpeedButton1: TD2SpeedButton;
    SpeedButton2: TD2SpeedButton;
    SpeedButton3: TD2SpeedButton;
    ListBox1: TD2ListBox;
    Layout1: TD2Layout;
    tempItem: TD2ListBoxItem;
    Image2: TD2Image;
    Text3: TD2Text;
    Rectangle3: TD2Rectangle;
    Text4: TD2Text;
    list: TD2Rectangle;
    d2Resources1: TD2Resources;
    procedure SpeedButton1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmIM: TfrmIM;

implementation


{$R *.lfm}

const
  Names: array [0..39] of string = (
   'stern',
   'anac',
   'taboo',
   'Isabella',
   'elena',
   'Emma',
   'Joshua',
   'Ava',
   'Daniel',
   'Madison',
   'Christopher',
   'Sophia',
   'Anthony',
   'Olivia',
   'William',
   'Abigail',
   'ethared',
   'vaha',
   'Andrew',
   'tata',
   'Alexander',
   'Addison',
   'David',
   'Samantha',
   'Joseph',
   'Ashley',
   'Noah',
   'Alyssa',
   'James',
   'Mia',
   'Ryan',
   'Chloe',
   'Logan',
   'Natalie',
   'Jayden',
   'Sarah',
   'John',
   'Alexis',
   'Nicholas',
   'Grace');

procedure TfrmIM.SpeedButton1Click(Sender: TObject);
begin
  Application.Terminate;
end;

procedure TfrmIM.FormCreate(Sender: TObject);
var
  i: integer;
  Item: TD2ListBoxItem;
begin
  { create items }
  for i := 0 to 20 do
  begin
    Item := TD2ListBoxItem(tempItem.Clone(Self));
    Item.Parent := ListBox1;
    Item.Binding['name'] := LowerCase(Names[random(Length(Names))]);
    Item.Binding['text'] := 'I''m on SMS';
  end;
end;

procedure TfrmIM.SpeedButton3Click(Sender: TObject);
begin
  { change back fill }
  ShowBrushDialog(back.Fill, [d2BrushSolid, d2BrushGradient, d2BrushBitmap], false);
end;

end.
