{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit importform;

{$mode Delphi}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, FileUtil, orca_scene2d, orca_scene3d;

type
  TForm2 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    ToolBar1: TD2ToolBar;
    ToolPathButton1: TD2ToolPathButton;
    Label1: TD2Label;
    Light1: TD3Light;
    ImportMesh: TD3Dummy;
    OpenDialog1: TOpenDialog;
    cameraz: TD3Dummy;
    camerax: TD3Dummy;
    camera: TD3Camera;
    procedure ToolPathButton1Click(Sender: TObject);
    procedure Root1MouseMove(Sender: TObject; Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure Root1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
    procedure Root1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TD3Point; var Handled: Boolean);
  private
    { Private declarations }
    Down: TD2Point;
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

uses File3DS, Types3DS;

{$R *.lfm}

type
  TSmoothIndexEntry = array[0..31] of Cardinal;
  PSmoothIndexArray = ^TSmoothIndexArray;
  TSmoothIndexArray = array[0..System.MaxInt shr 8] of TSmoothIndexEntry;

var
  Vertexs: array of TD3TexVertexNormal;
  Indexs: array of Word;

procedure Load3DSFromStream(FileName: string; const AStream: TStream; const AOwner: TD3VisualObject);
var
  Name: string;
  i, j: Integer;
  Face, Vertex, TargetVertex: Integer;
  CurrentIndex: Word;
  Vector1, Vector2, Normal: TD3Vector;
  standardNormalsOrientation: boolean;
  LastVCount, LastICount: integer;
  File3ds: TFile3DS;
  A, B, AA, BB: TD3Vector;
  Mesh: TD3Mesh;
  Size: TD3Vector;
  AboutSize: integer;
  mscale: single;
  M: PMaterial3DS;
  Marker: PByteArray;
  CurrentVertexCount: Integer;
  SmoothIndices: PSmoothIndexArray;
  SmoothingGroup: Cardinal;
  Matrix: TD3Matrix;
  Vec: TD3Vector;
  //---------------------------------------------------------------------------

   procedure DuplicateVertex(Index: Integer);
      // extends the vector and normal array by one entry and duplicates the vertex data given by Index
      // the marker and texture arrays will be extended too, if necessary
   begin
      // enhance vertex array
      SetLength(Vertexs, Length(Vertexs) + 1);
      Vertexs[Length(Vertexs) - 1] := Vertexs[Index];
      Vertexs[Length(Vertexs) - 1].nx := 0;
      Vertexs[Length(Vertexs) - 1].ny := 0;
      Vertexs[Length(Vertexs) - 1].nz := 0;
      // enhance smooth index array
      ReallocMem(SmoothIndices, (CurrentVertexCount + 1) * SizeOf(TSmoothIndexEntry));
      FillChar(SmoothIndices[CurrentVertexCount], SizeOf(TSmoothIndexEntry), $FF);
      // enhance marker array
      if (CurrentVertexCount div 8) <> ((CurrentVertexCount + 1) div 8) then begin
         ReallocMem(Marker, ((CurrentVertexCount + 1) div 8) + 1);
         Marker[(CurrentVertexCount div 8) + 1]:=0;
      end;
      System.Inc(CurrentVertexCount); 
   end;
   {$IFNDEF FPC}
   procedure StoreSmoothIndex(ThisIndex, SmoothingGroup, NewIndex: Cardinal; P: Pointer);
      // Stores new vertex index (NewIndex) into the smooth index array of vertex ThisIndex
      // using field SmoothingGroup, which must not be 0.
      // For each vertex in the vertex array (also for duplicated vertices) an array of 32 cardinals
      // is maintained (each for one possible smoothing group. If a vertex must be duplicated because
      // it has no smoothing group or a different one then the index of the newly created vertex is
      // stored in the SmoothIndices to avoid loosing the conjunction between not yet processed vertices
      // and duplicated vertices.
      // Note: Only one smoothing must be assigned per vertex. Some available models break this rule and
      //       have more than one group assigned to a face. To make the code fail safe the group ID
      //       is scanned for the lowest bit set.
   asm
                   PUSH EBX
                   BSF EBX, Ed3                  // determine smoothing group index (convert flag into an index)
                   MOV Ed3, [P]                  // get address of index array
                   SHL EAX, 7                    // ThisIndex * SizeOf(TSmoothIndexEntry)
                   ADD EAX, Ed3
                   LEA Ed3, [4 * EBX + EAX]      // Address of array + vertex index + smoothing group index
                   MOV [Ed3], ECX
                   POP EBX
   end;
   function IsVertexMarked(P: Pointer; Index: Integer): Boolean; assembler;
      // tests the Index-th bit, returns True if set else False
   asm
                     BT [EAX], Ed3
                     SETC AL
   end;
   function GetSmoothIndex(ThisIndex, SmoothingGroup: Cardinal; P: Pointer): Integer;
      // Retrieves the vertex index for the given index and smoothing group.
      // This redirection is necessary because a vertex might have been duplicated.
   asm
                   PUSH EBX
                   BSF EBX, Ed3                  // determine smoothing group index
                   SHL EAX, 7                    // ThisIndex * SizeOf(TSmoothIndexEntry)
                   ADD EAX, ECX
                   LEA ECX, [4 * EBX + EAX]      // Address of array + vertex index + smoothing group index
                   MOV EAX, [ECX]
                   POP EBX
   end;
   //---------------------------------------------------------------------------
   function MarkVertex(P: Pointer; Index: Integer): Boolean; assembler;
      // sets the Index-th bit and return True if it was already set else False
   asm
                     BTS [EAX], Ed3
                     SETC AL
   end;
   {$ELSE}
    procedure DivMod(dividend : Integer; divisor: Word; var result, remainder : Word);
    begin
      Result:=Dividend div Divisor;
      Remainder:=Dividend mod Divisor;
    end;

   function IsVertexMarked(P: PByteArray; Index: word): Boolean; inline;
      // tests the Index-th bit, returns True if set else False
   var mi : word;
   begin
     DivMod(index,8,mi,index);
     result:=(((p^[mi] shr Index) and 1) = 1);
   end;
   function MarkVertex(P: PByteArray; Index: word): Boolean;  inline;
      // sets the Index-th bit and return True if it was already set else False
   var mi : word;
   begin
     DivMod(index,8,mi,index);
     result:=(((p^[mi] shr Index) and 1) = 1);
     if not(result) then p^[mi]:=p^[mi] or (1 shl index);
   end;
   procedure StoreSmoothIndex(ThisIndex, SmoothingGroup, NewIndex: Cardinal; P: PSmoothIndexArray); inline;
      // Stores new vertex index (NewIndex) into the smooth index array of vertex ThisIndex
      // using field SmoothingGroup, which must not be 0.
      // For each vertex in the vertex array (also for duplicated vertices) an array of 32 cardinals
      // is maintained (each for one possible smoothing group. If a vertex must be duplicated because
      // it has no smoothing group or a different one then the index of the newly created vertex is
      // stored in the SmoothIndices to avoid loosing the conjunction between not yet processed vertices
      // and duplicated vertices.
      // Note: Only one smoothing must be assigned per vertex. Some available models break this rule and
      //       have more than one group assigned to a face. To make the code fail safe the group ID
      //       is scanned for the lowest bit set.
   var i : word;
   begin
     i:=0;
     while SmoothingGroup and (1 shl i) = 0 do inc(i);
     p^[ThisIndex,i]:=NewIndex;
   end;
   function GetSmoothIndex(ThisIndex, SmoothingGroup: Cardinal; P: PSmoothIndexArray): Integer; inline;
      // Retrieves the vertex index for the given index and smoothing group.
      // This redirection is necessary because a vertex might have been duplicated.
   var i : word;
   begin
     i:=0;
     while SmoothingGroup and (1 shl i) = 0 do inc(i);
     result:=integer(p^[ThisIndex,i]);
   end;
   {$ENDIF}

var
  Texture: TD3BitmapObject;
begin
  if AOwner.Scene <> nil then
    AOwner.Scene.BeginUpdate;
  AOwner.DeleteChildren;
  AOwner.Visible := false;

  File3ds := TFile3DS.Create;
  with File3ds do
    try
      LoadFromStream(aStream);
      standardNormalsOrientation := true; //not (NormalsOrientation = mnoDefault);
      LastVCount := 0;
      LastICount := 0;
      Matrix := IdentityMatrix;
      { calc global size }
      for i := 0 to Objects.MeshCount-1 do
        with PMesh3DS(Objects.Mesh[I])^ do
        begin
          if IsHidden or (NVertices<3) then Continue;
            Name := PMesh3DS(Objects.Mesh[I])^.NameStr;
            // make a copy of the vertex data, this must always be available
            SetLength(Vertexs, LastVCount + NVertices);

            Matrix := IdentityMatrix;
            Move(LocMatrix[0], Matrix.M[0], 3 * 4);
            Move(LocMatrix[3], Matrix.M[1], 3 * 4);
            Move(LocMatrix[6], Matrix.M[2], 3 * 4);
            Move(LocMatrix[9], Matrix.M[3], 3 * 4);
            //InvertMatrix(Matrix);
            if d3MatrixDeterminant(Matrix) < 0 then
            begin
              InvertMatrix(Matrix);
            end;
            if NTextVerts > 0 then
            begin
               for j := 0 to NVertices-1 do
               begin
                 Vec := d3Vector(VertexArray[j].x, VertexArray[j].y, VertexArray[j].z, 1);
                 //Vec := d3VectorTransform(Vec, Matrix);
                 Vertexs[LastVCount + j].x := Vec.x;
                 Vertexs[LastVCount + j].y := Vec.y;
                 Vertexs[LastVCount + j].z := Vec.z;
                 Vertexs[LastVCount + j].tu := TextArray[j].U;
                 Vertexs[LastVCount + j].tv := 1 - TextArray[j].V;
               end;
            end
            else
            begin
               for j := 0 to NVertices - 1 do
               begin
                 Vec := d3Vector(VertexArray[j].x, VertexArray[j].y, VertexArray[j].z, 1);
                 //Vec := d3VectorTransform(Vec, Matrix);
                 Vertexs[LastVCount + j].x := Vec.x;
                 Vertexs[LastVCount + j].y := Vec.y;
                 Vertexs[LastVCount + j].z := Vec.z;
               end;
            end;
          LastVCount := LastVCount + NVertices;
          LastICount := LastICount + NFaces * 3;
        end;
      AboutSize := (LastVCount * 100) + (LastICount * 20) + (Objects.MeshCount * 300);
      A := d3Vector($FFFF, $FFFF, $FFFF);
      B := d3Vector(-$FFFF, -$FFFF, -$FFFF);
      for j := 0 to High(Vertexs) do
      begin
        if Vertexs[j].x < A.x then A.x := Vertexs[j].x;
        if Vertexs[j].y < A.y then A.y := Vertexs[j].y;
        if Vertexs[j].z < A.z then A.z := Vertexs[j].z;
        if Vertexs[j].x > B.x then B.x := Vertexs[j].x;
        if Vertexs[j].y > B.y then B.y := Vertexs[j].y;
        if Vertexs[j].z > B.z then B.z := Vertexs[j].z;
      end;
      if Abs(B.z - A.z) < Epsilon then
        B.z := A.z + 0.01;
      mscale := 1 / MaxFloat(MaxFloat(Abs(B.x - A.x), Abs(B.y - A.y)), Abs(B.z - A.z));
      { create meshs }
      LastVCount := 0;
      LastICount := 0;
      for i := 0 to Objects.MeshCount-1 do
        with PMesh3DS(Objects.Mesh[I])^ do                                     
        begin
          if IsHidden or (NVertices<3) then Continue;
            Name := PMesh3DS(Objects.Mesh[I])^.NameStr;
            // make a copy of the vertex data, this must always be available
            SetLength(Vertexs, LastVCount + NVertices);

            Matrix := IdentityMatrix;
            Move(LocMatrix[0], Matrix.M[0], 3 * 4);
            Move(LocMatrix[3], Matrix.M[1], 3 * 4);
            Move(LocMatrix[6], Matrix.M[2], 3 * 4);
            Move(LocMatrix[9], Matrix.M[3], 3 * 4);

            if NTextVerts > 0 then
            begin
               for j := 0 to NVertices-1 do
               begin
                 Vec := d3Vector(VertexArray[j].x, VertexArray[j].y, VertexArray[j].z, 1);
                 //Vec := d3VectorTransform(Vec, Matrix);
                 Vertexs[LastVCount + j].x := Vec.x;
                 Vertexs[LastVCount + j].y := Vec.y;
                 Vertexs[LastVCount + j].z := Vec.z;
                 Vertexs[LastVCount + j].tu := TextArray[j].U;
                 Vertexs[LastVCount + j].tv := 1 - TextArray[j].V;
               end;
            end
            else
            begin
               for j := 0 to NVertices - 1 do
               begin
                 Vec := d3Vector(VertexArray[j].x, VertexArray[j].y, VertexArray[j].z, 1);
                 //Vec := d3VectorTransform(Vec, Matrix);
                 Vertexs[LastVCount + j].x := Vec.x;
                 Vertexs[LastVCount + j].y := Vec.y;
                 Vertexs[LastVCount + j].z := Vec.z;
               end;
            end;
           // allocate memory for the smoothindices and the marker array
           CurrentVertexCount := NVertices;
           Marker := AllocMem((NVertices div 8) + 1); // one bit for each vertex
           GetMem(SmoothIndices, NVertices * SizeOf(TSmoothIndexEntry));
           for face:=0 to NFaces-1 do
             with FaceArray[Face] do
             begin
               // normal vector for the face
               vector1 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V1].x, Vertexs[LastVCount + V1].y, Vertexs[LastVCount + V1].z),
                 d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
               vector2 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V3].x, Vertexs[LastVCount + V3].y, Vertexs[LastVCount + V3].z),
                 d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
               if standardNormalsOrientation then
                 Normal := d3VectorCrossProduct(Vector1, Vector2)
               else
                 Normal := d3VectorCrossProduct(Vector2, Vector1);
               for Vertex := 0 to 2 do
               begin
                  // copy current index for faster access
                  CurrentIndex := FaceRec[Vertex];
                  Vertexs[LastVCount + CurrentIndex].nx := Normal.X;
                  Vertexs[LastVCount + CurrentIndex].ny := Normal.Y;
                  Vertexs[LastVCount + CurrentIndex].nz := Normal.Z;
               end;
             end;
           { Skip smoothing }
           Marker := AllocMem((NVertices div 8) + 1); // one bit for each vertex
           GetMem(SmoothIndices, NVertices * SizeOf(TSmoothIndexEntry));
           if (SmoothArray = nil) then
           begin
              // no smoothing groups to consider
              for face:=0 to NFaces-1 do
                with FaceArray[Face] do
                begin
                 // normal vector for the face
                 vector1 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V1].x, Vertexs[LastVCount + V1].y, Vertexs[LastVCount + V1].z),
                   d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
                 vector2 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V3].x, Vertexs[LastVCount + V3].y, Vertexs[LastVCount + V3].z),
                   d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
                 if standardNormalsOrientation then
                   Normal := d3VectorCrossProduct(Vector1, Vector2)
                 else
                   Normal := d3VectorCrossProduct(Vector2, Vector1);
                 // go for each vertex in the current face
                 for Vertex := 0 to 2 do
                 begin
                    // copy current index for faster access
                    CurrentIndex := FaceRec[Vertex];

                    // already been touched?
                    if IsVertexMarked(Marker, CurrentIndex) then
                    begin
                       // already touched vertex must be duplicated
                       DuplicateVertex(LastVCount + CurrentIndex);
                       FaceRec[Vertex] := CurrentVertexCount-1;
                       Vertexs[LastVCount + CurrentVertexCount-1].nx := Normal.X;
                       Vertexs[LastVCount + CurrentVertexCount-1].ny := Normal.Y;
                       Vertexs[LastVCount + CurrentVertexCount-1].nz := Normal.Z;
                    end
                    else
                    begin
                       // not yet touched, so just store the normal
                       Vertexs[LastVCount + CurrentIndex].nx := Normal.X;
                       Vertexs[LastVCount + CurrentIndex].ny := Normal.Y;
                       Vertexs[LastVCount + CurrentIndex].nz := Normal.Z;
                       MarkVertex(Marker, CurrentIndex);
                    end;
                 end;
              end;
           end
           else
           begin
              // smoothing groups are to be considered
              for Face:=0 to NFaces-1 do
              with FaceArray[Face] do
              begin
                 // normal vector for the face
                 vector1 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V1].x, Vertexs[LastVCount + V1].y, Vertexs[LastVCount + V1].z),
                   d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
                 vector2 := d3VectorSubtract(d3Vector(Vertexs[LastVCount + V3].x, Vertexs[LastVCount + V3].y, Vertexs[LastVCount + V3].z),
                   d3Vector(Vertexs[LastVCount + V2].x, Vertexs[LastVCount + V2].y, Vertexs[LastVCount + V2].z));
                 if standardNormalsOrientation then
                   Normal := d3VectorCrossProduct(Vector1, Vector2)
                 else
                   Normal := d3VectorCrossProduct(Vector2, Vector1);
                 SmoothingGroup := SmoothArray[Face];
                 // go for each vertex in the current face
                 for Vertex := 0 to 2 do
                 begin
                    // copy current index for faster access
                    currentIndex := FaceRec[Vertex];
                    // Has vertex already been touched?
                    if IsVertexMarked(Marker, currentIndex) then
                    begin
                       // check smoothing group
                       if SmoothingGroup = 0 then
                       begin
                          // no smoothing then just duplicate vertex
                          DuplicateVertex(LastVCount + CurrentIndex);
                          FaceRec[Vertex] := CurrentVertexCount - 1;
                          Vertexs[LastVCount + CurrentVertexCount-1].nx := Normal.X;
                          Vertexs[LastVCount + CurrentVertexCount-1].ny := Normal.Y;
                          Vertexs[LastVCount + CurrentVertexCount-1].nz := Normal.Z;
                          // mark new vertex also as touched
                          MarkVertex(Marker, CurrentVertexCount - 1);
                       end
                       else
                       begin
                          // this vertex must be smoothed, check if there's already
                          // a (duplicated) vertex for this smoothing group
                          TargetVertex := GetSmoothIndex(CurrentIndex, SmoothingGroup, SmoothIndices);
                          if TargetVertex < 0 then
                          begin
                             // vertex has not yet been duplicated for this smoothing
                             // group, so do it now
                             DuplicateVertex(LastVCount + CurrentIndex);
                             FaceRec[Vertex] := CurrentVertexCount - 1;
                             Vertexs[LastVCount + CurrentVertexCount-1].nx := Normal.X;
                             Vertexs[LastVCount + CurrentVertexCount-1].ny := Normal.Y;
                             Vertexs[LastVCount + CurrentVertexCount-1].nz := Normal.Z;
                             StoreSmoothIndex(CurrentIndex, SmoothingGroup, CurrentVertexCount - 1, SmoothIndices);
                             StoreSmoothIndex(CurrentVertexCount - 1, SmoothingGroup, CurrentVertexCount - 1, SmoothIndices);
                             // mark new vertex also as touched
                             MarkVertex(Marker, CurrentVertexCount - 1);
                          end
                          else
                          begin
                             // vertex has already been duplicated,
                             // so just add normal vector to other vertex...
                             with d3VectorAdd(d3Vector(Vertexs[TargetVertex].nx, Vertexs[TargetVertex].ny, Vertexs[TargetVertex].nz), Normal) do
                             begin
                               Vertexs[LastVCount + TargetVertex].nX := X;
                               Vertexs[LastVCount + TargetVertex].nY := Y;
                               Vertexs[LastVCount + TargetVertex].nZ := Z;
                             end;
                             // ...and tell which new vertex has to be used from now on
                             FaceRec[Vertex] := TargetVertex;
                          end;
                       end;
                    end
                    else
                    begin
                       // vertex not yet touched, so just store the normal
                       Vertexs[LastVCount + CurrentIndex].nx := Normal.X;
                       Vertexs[LastVCount + CurrentIndex].ny := Normal.Y;
                       Vertexs[LastVCount + CurrentIndex].nz := Normal.Z;
                       // initialize smooth indices for this vertex
                       FillChar(SmoothIndices[CurrentIndex], SizeOf(TSmoothIndexEntry), $FF);
                       if SmoothingGroup <> 0 then
                          StoreSmoothIndex(CurrentIndex, SmoothingGroup, CurrentIndex, SmoothIndices);
                       MarkVertex(Marker, CurrentIndex);
                    end;
                 end;
              end;
          end;
          FreeMem(SmoothIndices);
          FreeMem(Marker);
          // Create indexs
          SetLength(Indexs, LastICount + NFaces * 3);
          for j := 0 to NFaces-1 do
          begin
            Indexs[LastICount + (j * 3) + 0] := LastVCount + FaceArray[j].V1;
            Indexs[LastICount + (j * 3) + 1] := LastVCount + FaceArray[j].V3;
            Indexs[LastICount + (j * 3) + 2] := LastVCount + FaceArray[j].V2;
          end;
          { invert normals }
          for j := 0 to High(Vertexs) do
          begin
            Vertexs[j].nx := -Vertexs[j].nx;
            Vertexs[j].ny := -Vertexs[j].ny;
            Vertexs[j].nz := -Vertexs[j].nz;
          end;
          { Local mesh size }
          AA := d3Vector($FFFF, $FFFF, $FFFF);
          BB := d3Vector(-$FFFF, -$FFFF, -$FFFF);
          for j := 0 to High(Vertexs) do
          begin
            if Vertexs[j].x < AA.x then AA.x := Vertexs[j].x;
            if Vertexs[j].y < AA.y then AA.y := Vertexs[j].y;
            if Vertexs[j].z < AA.z then AA.z := Vertexs[j].z;
            if Vertexs[j].x > BB.x then BB.x := Vertexs[j].x;
            if Vertexs[j].y > BB.y then BB.y := Vertexs[j].y;
            if Vertexs[j].z > BB.z then BB.z := Vertexs[j].z;
          end;
          if Abs(BB.z - AA.z) < Epsilon then
            BB.z := AA.z + 0.01;
          // Create mesh
          for j := 0 to High(Vertexs) do
          begin
            Vertexs[j].x := Vertexs[j].x - ((BB.x + AA.x) / 2);
            Vertexs[j].y := Vertexs[j].y - ((BB.y + AA.y) / 2);
            Vertexs[j].z := Vertexs[j].z - ((BB.z + AA.z) / 2);

            Vertexs[j].x := Vertexs[j].x / (Abs(BB.x - AA.x));
            Vertexs[j].y := Vertexs[j].y / (Abs(BB.y - AA.y));
            Vertexs[j].z := Vertexs[j].z / (Abs(BB.z - AA.z));
          end;
          Size := d3VectorScale(d3Vector(Abs(BB.x - AA.x), Abs(BB.y - AA.y), Abs(BB.z - AA.z)), mscale);
          { Create mesh }
          Mesh := TD3Mesh.Create(AOwner.Owner);
          with Mesh do
          begin
            Parent := AOwner;
            SetLength(Data.MeshVertices, Length(Vertexs));
            Move(Vertexs[0], Data.MeshVertices[0], Length(Vertexs) * SizeOf(Vertexs[0]));
            SetLength(Data.MeshIndices, Length(Indexs));
            Move(Indexs[0], Data.MeshIndices[0], Length(Indexs) * SizeOf(Indexs[0]));
            Width := Size.X;
            Height := Size.Y;
            Depth := Size.Z;
            Position.X := (AA.x + (Abs(BB.x - AA.x) / 2) - ((B.x + A.x) / 2)) * mscale;
            Position.Y := (AA.y + (Abs(BB.y - AA.y) / 2) - ((B.y + A.y) / 2)) * mscale;
            Position.Z := (AA.z + (Abs(BB.z - AA.z) / 2) - ((B.z + A.z) / 2)) * mscale;
            Align := d3Scale;
            TagString := NameStr;
            { material }
            if (Materials <> nil) and (NMats > 0) then
            begin
              M := Materials.MaterialByName[MatArray[0].NameStr];
              if M <> nil then
              begin
                Mesh.Material.NativeDiffuse := d3Color(Trunc(M.Diffuse.R * $FF), Trunc(M.Diffuse.G * $FF), Trunc(M.Diffuse.B * $FF), $FF);
                if (M.Texture.Map.NameStr <> '') and FileExistsUTF8(ExtractFilePath(FileName) + M.Texture.Map.NameStr) { *Converted from FileExists*  } then
                begin
                  Texture := TD3BitmapObject.Create(AOwner);
                  Texture.Parent := AOwner;
                  Texture.Bitmap.LoadFromFile(ExtractFilePath(FileName) + M.Texture.Map.NameStr);
                  Texture.ResourceName := M.Texture.Map.NameStr;
                  Mesh.Material.Bitmap := M.Texture.Map.NameStr;
                end;
              end;
            end;
            Locked := true;
            HitTest := false;
          end;
        end;
    Size := d3VectorNormalize(d3Vector(AOwner.Width, AOwner.Height, AOwner.Depth));

    mscale := 1 / AOwner.Width;
    if (Size.Y >= Size.X) and (Size.Y >= Size.Z) then
      mscale := 1 / AOwner.Height;
    if (Size.Z >= Size.X) and (Size.Z >= Size.Y) then
      mscale := 1 / AOwner.Depth;

    AOwner.Width := AOwner.Width * mscale;
    AOwner.Height := AOwner.Height * mscale;
    AOwner.Depth := AOwner.Depth * mscale;
  finally
    Free;
    AOwner.Visible := true;
    if AOwner.Scene <> nil then
      AOwner.Scene.EndUpdate;
  end;
end;



procedure TForm2.ToolPathButton1Click(Sender: TObject);
var
  S: TStream;
begin
  { Import }
  if OpenDialog1.Execute then
  begin
    S := TFileStream.Create(OpenDialog1.FileName, fmOpenRead);
    Load3DSFromStream(OpenDialog1.FileName, S, ImportMesh);
    S.Free;
  end;
end;

procedure TForm2.Root1MouseMove(Sender: TObject; Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  if (ssLeft in Shift) then
  begin
    { rotate Z }
    CameraZ.RotateAngle.Z := CameraZ.RotateAngle.Z + ((X - Down.X) * 0.3);
    { rotate X }
    CameraX.RotateAngle.X := CameraX.RotateAngle.X + ((Y - Down.Y) * 0.3);
    Down := d2Point(X, Y);
  end;
end;

procedure TForm2.Root1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single; rayPos, rayDir: TD3Vector);
begin
  Down := d2Point(X, Y);
end;

procedure TForm2.Root1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TD3Point; var Handled: Boolean);
begin
  Camera.Position.Vector := d3VectorAdd(Camera.Position.Vector, d3VectorScale(d3Vector(0, 1, 0), (WheelDelta / 120) * 0.3));
end;

end.
