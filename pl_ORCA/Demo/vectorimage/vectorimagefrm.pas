{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit vectorimagefrm;

  {$mode objfpc}{$H+}

interface

uses

  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, StdCtrls;

type
  TfrmVectorImage = class(TForm)
    d2Scene1: TD2Scene;
    VectorImageRoot: TD2Background;
    LionRoot: TD2Panel;
    Flowroot: TD2Panel;
    Text1: TD2Label;
    Text2: TD2Label;
    Text3: TD2Label;
    TextBox1: TD2TextBox;
    Flow: TD2Path;
    Text4: TD2Label;
    Text5: TD2Label;
    Button1: TD2Button;
    d2Resources1: TD2Resources;
    Lion: TD2ScaledLayout;
    procedure FlowrootMouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TD2Point; var Handled: Boolean);
    procedure LionRootMouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TD2Point; var Handled: Boolean);
    procedure TextBox1Change(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmVectorImage: TfrmVectorImage;
  
implementation

{$R *.lfm}

procedure TfrmVectorImage.FlowrootMouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TD2Point;
  var Handled: Boolean);
begin
  if (WheelDelta < 0) and (Lion.Width < 4) then Exit;

  Flow.Width := Flow.Width + (WheelDelta * 0.001 * Flow.Width);
  Flow.Height := Flow.Height + (WheelDelta * 0.001 * Flow.Height);
  FlowRoot.Realign;
end;

procedure TfrmVectorImage.LionRootMouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TD2Point;
  var Handled: Boolean);
begin
  if (WheelDelta < 0) and (Lion.Width < 4) then Exit;

  Lion.Width := Lion.Width + (WheelDelta * 0.001 * Lion.Width);
  Lion.Height := Lion.Height + (WheelDelta * 0.001 * Lion.Height);
  LionRoot.Realign;
end;

procedure TfrmVectorImage.TextBox1Change(Sender: TObject);
begin
  Flow.Data.Data := TextBox1.Text;
end;

procedure TfrmVectorImage.Button1Click(Sender: TObject);
begin
  SelectInDesign(Flow.Fill, Flow);
end;

end.
