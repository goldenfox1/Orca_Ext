{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit adveffectsfrm;

{$mode objfpc}{$H+}

interface

uses 
  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,  StdCtrls, orca_scene2d;

type
  TfrmAdvEffects = class(TForm)
    d2Scene1: TD2Scene;
    VectorImageRoot: TD2Background;
    d2Resources1: TD2Resources;
    TabControl1: TD2TabControl;
    TabItem1: TD2TabItem;
    Layout1: TD2Layout;
    TabItem2: TD2TabItem;
    Layout2: TD2Layout;
    ReflectionEffect1: TD2ReflectionEffect;
    GlowEffect1: TD2GlowEffect;
    Image2: TD2Image;
    Text1: TD2Text;
    Text3: TD2Text;
    TabItem3: TD2TabItem;
    Layout3: TD2Layout;
    TabItem4: TD2TabItem;
    Layout4: TD2Layout;
    Image1: TD2Image;
    Image3: TD2Image;
    Image4: TD2Image;
    GlowEffect2: TD2GlowEffect;
    GlowEffect3: TD2GlowEffect;
    Text2: TD2Text;
    Text4: TD2Text;
    Text5: TD2Text;
    CheckBox1: TD2CheckBox;
    procedure CheckBox1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAdvEffects: TfrmAdvEffects;

implementation

{$R *.lfm}

procedure TfrmAdvEffects.CheckBox1Change(Sender: TObject);
begin
  TabItem2.Enabled := CheckBox1.IsChecked;
end;

end.
