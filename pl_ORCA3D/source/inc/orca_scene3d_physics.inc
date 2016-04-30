{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//============ TD3Physics ============================
constructor TD3Physics.Create(const Scene:TD3Scene);
begin
  inherited Create;
  FScene:=Scene;
end;

destructor TD3Physics.Destroy;
begin
  inherited;
end;

//================ TD3DummyPhysics =================

constructor TD3DummyPhysics.Create(const Scene:TD3Scene);
begin
  inherited;

end;

destructor TD3DummyPhysics.Destroy;
begin

  inherited;
end;

function  TD3DummyPhysics.CreateWorld:integer;
begin
  result:=0;
end;

procedure TD3DummyPhysics.DestroyWorld(const AWorld:integer);
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.UpdateWorld(const AWorld:integer; const DeltaTime:single);
begin
  // nothing is dummy..
end;

function  TD3DummyPhysics.Collise(const AOwner:TD3VisualObject; var AList:TList):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.ComplexCollise(const AOwner:TD3VisualObject; var AList:TList):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.ObjectByBody(const Body:integer):TD3VisualObject;
begin
  result:=nil;
end;

function  TD3DummyPhysics.CreateBox(const AOwner:TD3VisualObject; const ASize:TD3Vector):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.CreateSphere(const AOwner:TD3VisualObject; const ASize:TD3Vector):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.CreateCone(const AOwner:TD3VisualObject; const ASize:TD3Vector):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.CreateCylinder(const AOwner:TD3VisualObject;  const ASize:TD3Vector):integer;
begin
  result:=0;
end;

function  TD3DummyPhysics.GetBodyMatrix(const Body:integer):TD3Matrix;
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.DestroyBody(var ABody:integer);
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.AddForce(const AOwner:TD3VisualObject; const Force:TD3Vector);
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.Explode(const AWorld:integer; const Position:TD3Vector; const Radius,Force:single);
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.Wind(const AWorld:integer; const Dir:TD3Vector; const Force:single);
begin
  // nothing is dummy..
end;

procedure TD3DummyPhysics.SetBodyMatrix(const Body:integer; const M:TD3Matrix);
begin
  // nothing is dummy..
end;

