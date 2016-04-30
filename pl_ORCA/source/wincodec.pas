{***************************************************
 ORCA 2D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit Wincodec;

interface

{$ALIGN ON}
{$MINENUMSIZE 4}

uses Windows, ActiveX;

type
{ IPropertyBag2 interface }

  tagPROPBAG2 = record
    dwType: DWORD;
    vt: TVarType;
    cfType: TClipFormat;
    dwHint: DWORD;
    pstrName: POleStr;
    clsid: TCLSID;
  end;
  TPropBag2 = tagPROPBAG2;
  PPropBag2 = ^TPropBag2;

  IPropertyBag2 = interface(IUnknown)
    ['{22F55882-280B-11d0-A8A9-00A0C90C2004}']
    function Read(pPropBag: PPropBag2; pErrLog: IErrorLog;pvarValue: PVariant; phrError: PHResult): HRESULT; stdcall;
    function Write(cProperties: ULONG; pPropBag: PPropBag2;pvarValue: PVariant): HRESULT; stdcall;
    function CountProperties(var pcProperties: ULONG): HRESULT; stdcall;
    function GetPropertyInfo(iProperty, cProperties: ULONG; pPropBag: PPropBag2; var pcProperties: ULONG): HRESULT; stdcall;
    function LoadObject(pstrName:POleStr; dwHint: DWORD; pUnkObject: IUnknown;pErrLog: IErrorLog): HRESULT; stdcall;
  end;

const
  SID_IWICPalette                         = '{00000040-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmapSource                    = '{00000120-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICFormatConverter                 = '{00000301-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmapScaler                    = '{00000302-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmapClipper                   = '{E4FBCF03-223D-4e81-9333-D635556DD1B5}';
  SID_IWICBitmapFlipRotator               = '{5009834F-2D6A-41ce-9E1B-17C5AFF7A782}';
  SID_IWICBitmapLock                      = '{00000123-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmap                          = '{00000121-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICColorTransform                  = '{B66F034F-D0E2-40ab-B436-6DE39E321A94}';
  SID_IWICColorContext                    = '{3C613A02-34B2-44ea-9A7C-45AEA9C6FD6D}';
  SID_IWICFastMetadataEncoder             = '{B84E2C09-78C9-4AC4-8BD3-524AE1663A2F}';
  SID_IWICStream                          = '{135FF860-22B7-4ddf-B0F6-218F4F299A43}';
  SID_IWICEnumMetadataItem                = '{DC2BB46D-3F07-481E-8625-220C4AEDBB33}';
  SID_IWICMetadataQueryReader             = '{30989668-E1C9-4597-B395-458EEDB808DF}';
  SID_IWICMetadataQueryWriter             = '{A721791A-0DEF-4d06-BD91-2118BF1DB10B}';
  SID_IWICBitmapEncoder                   = '{00000103-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmapFrameEncode               = '{00000105-a8f2-4877-ba0a-fd2b6645fb94}';
  SID_IWICBitmapDecoder                   = '{9EDDE9E7-8DEE-47ea-99DF-E6FAF2ED44BF}';
  SID_IWICBitmapSourceTransform           = '{3B16811B-6A43-4ec9-B713-3D5A0C13B940}';
  SID_IWICBitmapFrameDecode               = '{3B16811B-6A43-4ec9-A813-3D930C13B940}';
  SID_IWICProgressiveLevelControl         = '{DAAC296F-7AA5-4dbf-8D15-225C5976F891}';
  SID_IWICProgressCallback                = '{4776F9CD-9517-45FA-BF24-E89C5EC5C60C}';
  SID_IWICBitmapCodecProgressNotification = '{64C1024E-C3CF-4462-8078-88C2B11C46D9}';
  SID_IWICComponentInfo                   = '{23BC3F0A-698B-4357-886B-F24D50671334}';
  SID_IWICFormatConverterInfo             = '{9F34FB65-13F4-4f15-BC57-3726B5E53D9F}';
  SID_IWICBitmapCodecInfo                 = '{E87A44C4-B76E-4c47-8B09-298EB12A2714}';
  SID_IWICBitmapEncoderInfo               = '{94C9B4EE-A09F-4f92-8A1E-4A9BCE7E76FB}';
  SID_IWICBitmapDecoderInfo               = '{D8CD007F-D08F-4191-9BFC-236EA7F0E4B5}';
  SID_IWICPixelFormatInfo                 = '{E8EDA601-3D48-431a-AB44-69059BE88BBE}';
  SID_IWICPixelFormatInfo2                = '{A9DB33A2-AF5F-43C7-B679-74F5984B5AA4}';
  SID_IWICImagingFactory                  = '{ec5ec8a9-c395-4314-9c77-54d7a935ff70}';
  SID_IWICDevelopRawNotificationCallback  = '{95c75a6e-3e8c-4ec2-85a8-aebcc551e59b}';
  SID_IWICDevelopRaw                      = '{fbec5e44-f7be-4b65-b7f8-c0c81fef026d}';

  IID_IWICPalette:                         TGUID = SID_IWICPalette;
  IID_IWICBitmapSource:                    TGUID = SID_IWICBitmapSource;
  IID_IWICFormatConverter:                 TGUID = SID_IWICFormatConverter;
  IID_IWICBitmapScaler:                    TGUID = SID_IWICBitmapScaler;
  IID_IWICBitmapClipper:                   TGUID = SID_IWICBitmapClipper;
  IID_IWICBitmapFlipRotator:               TGUID = SID_IWICBitmapFlipRotator;
  IID_IWICBitmapLock:                      TGUID = SID_IWICBitmapLock;
  IID_IWICBitmap:                          TGUID = SID_IWICBitmap;
  IID_IWICColorTransform:                  TGUID = SID_IWICColorTransform;
  IID_IWICColorContext:                    TGUID = SID_IWICColorContext;
  IID_IWICFastMetadataEncoder:             TGUID = SID_IWICFastMetadataEncoder;
  IID_IWICStream:                          TGUID = SID_IWICStream;
  IID_IWICEnumMetadataItem:                TGUID = SID_IWICEnumMetadataItem;
  IID_IWICMetadataQueryReader:             TGUID = SID_IWICMetadataQueryReader;
  IID_IWICMetadataQueryWriter:             TGUID = SID_IWICMetadataQueryWriter;
  IID_IWICBitmapEncoder:                   TGUID = SID_IWICBitmapEncoder;
  IID_IWICBitmapFrameEncode:               TGUID = SID_IWICBitmapFrameEncode;
  IID_IWICBitmapDecoder:                   TGUID = SID_IWICBitmapDecoder;
  IID_IWICBitmapSourceTransform:           TGUID = SID_IWICBitmapSourceTransform;
  IID_IWICBitmapFrameDecode:               TGUID = SID_IWICBitmapFrameDecode;
  IID_IWICProgressiveLevelControl:         TGUID = SID_IWICProgressiveLevelControl;
  IID_IWICProgressCallback:                TGUID = SID_IWICProgressCallback;
  IID_IWICBitmapCodecProgressNotification: TGUID = SID_IWICBitmapCodecProgressNotification;
  IID_IWICComponentInfo:                   TGUID = SID_IWICComponentInfo;
  IID_IWICFormatConverterInfo:             TGUID = SID_IWICFormatConverterInfo;
  IID_IWICBitmapCodecInfo:                 TGUID = SID_IWICBitmapCodecInfo;
  IID_IWICBitmapEncoderInfo:               TGUID = SID_IWICBitmapEncoderInfo;
  IID_IWICBitmapDecoderInfo:               TGUID = SID_IWICBitmapDecoderInfo;
  IID_IWICPixelFormatInfo:                 TGUID = SID_IWICPixelFormatInfo;
  IID_IWICPixelFormatInfo2:                TGUID = SID_IWICPixelFormatInfo2;
  IID_IWICImagingFactory:                  TGUID = SID_IWICImagingFactory;
  IID_IWICDevelopRawNotificationCallback:  TGUID = SID_IWICDevelopRawNotificationCallback;
  IID_IWICDevelopRaw:                      TGUID = SID_IWICDevelopRaw;

const
  WINCODEC_SDK_VERSION = $0236;
  CLSID_WICImagingFactory:           TGUID = '{CACAF262-9370-4615-A13B-9F5539DA4C0A}';
  GUID_VendorMicrosoft:              TGUID = '{F0E749CA-EDEF-4589-A73A-EE0E626A2A2B}';
  GUID_VendorMicrosoftBuiltIn:       TGUID = '{257A30FD-06B6-462B-AEA4-63F70B86E533}';
  CLSID_WICBmpDecoder:               TGUID = '{6B462062-7CBF-400D-9FDB-813DD10F2778}';
  CLSID_WICPngDecoder:               TGUID = '{389EA17B-5078-4CDE-B6EF-25C15175C751}';
  CLSID_WICIcoDecoder:               TGUID = '{C61BFCDF-2E0F-4AAD-A8D7-E06BAFEBCDFE}';
  CLSID_WICJpegDecoder:              TGUID = '{9456A480-E88B-43EA-9E73-0B2D9B71B1CA}';
  CLSID_WICGifDecoder:               TGUID = '{381DDA3C-9CE9-4834-A23E-1F98F8FC52BE}';
  CLSID_WICTiffDecoder:              TGUID = '{B54E85D9-FE23-499F-8B88-6ACEA713752B}';
  CLSID_WICWmpDecoder:               TGUID = '{A26CEC36-234C-4950-AE16-E34AACE71D0D}';
  CLSID_WICBmpEncoder:               TGUID = '{69BE8BB4-D66D-47C8-865A-ED1589433782}';
  CLSID_WICPngEncoder:               TGUID = '{27949969-876A-41D7-9447-568F6A35A4DC}';
  CLSID_WICJpegEncoder:              TGUID = '{1A34F5C1-4A5A-46DC-B644-1F4567E7A676}';
  CLSID_WICGifEncoder:               TGUID = '{114F5598-0B22-40A0-86A1-C83EA495ADBD}';
  CLSID_WICTiffEncoder:              TGUID = '{0131BE10-2001-4C5F-A9B0-CC88FAB64CE8}';
  CLSID_WICWmpEncoder:               TGUID = '{AC4CE3CB-E1C1-44CD-8215-5A1665509EC2}';
  GUID_ContainerFormatBmp:           TGUID = '{0AF1D87E-FCFE-4188-BDEB-A7906471CBE3}';
  GUID_ContainerFormatPng:           TGUID = '{1B7CFAF4-713F-473C-BBCD-6137425FAEAF}';
  GUID_ContainerFormatIco:           TGUID = '{A3A860C4-338F-4C17-919A-FBA4B5628F21}';
  GUID_ContainerFormatJpeg:          TGUID = '{19E4A5AA-5662-4FC5-A0C0-1758028E1057}';
  GUID_ContainerFormatTiff:          TGUID = '{163BCC30-E2E9-4F0B-961D-A3E9FDB788A3}';
  GUID_ContainerFormatGif:           TGUID = '{1F8A5601-7D4D-4CBD-9C82-1BC8D4EEB9A5}';
  GUID_ContainerFormatWmp:           TGUID = '{57A37CAA-367A-4540-916B-F183C5093A4B}';
  CLSID_WICImagingCategories:        TGUID = '{FAE3D380-FEA4-4623-8C75-C6B61110B681}';
  CATID_WICBitmapDecoders:           TGUID = '{7ED96837-96F0-4812-B211-F13C24117ED3}';
  CATID_WICBitmapEncoders:           TGUID = '{AC757296-3522-4E11-9862-C17BE5A1767E}';
  CATID_WICPixelFormats:             TGUID = '{2B46E70F-CDA7-473E-89F6-DC9630A2390B}';
  CATID_WICFormatConverters:         TGUID = '{7835EAE8-BF14-49D1-93CE-533A407B2248}';
  CATID_WICMetadataReader:           TGUID = '{05AF94D8-7174-4CD2-BE4A-4124B80EE4B8}';
  CATID_WICMetadataWriter:           TGUID = '{ABE3B9A4-257D-4B97-BD1A-294AF496222E}';
  CLSID_WICDefaultFormatConverter:   TGUID = '{1A3F11DC-B514-4B17-8C5F-2154513852F1}';
  CLSID_WICFormatConverterHighColor: TGUID = '{AC75D454-9F37-48F8-B972-4E19BC856011}';
  CLSID_WICFormatConverterNChannel:  TGUID = '{C17CABB2-D4A3-47D7-A557-339B2EFBD4F1}';
  CLSID_WICFormatConverterWMPhoto:   TGUID = '{9CB5172B-D600-46BA-AB77-77BB7E3A00D9}';

type
  WICColor = Cardinal;
  TWICColor = WICColor;
  PWICColor = ^TWicColor;

  WICRect = record
    X: Integer;
    Y: Integer;
    Width: Integer;
    Height: Integer;
  end;
  PWICRect = ^WICRect;


  WICInProcPointer = ^Byte;
  { $EXTERNALSYM WICInProcPointer}
  TWICInProcPointer = WICInProcPointer;
  PWICInProcPointer = ^TWICInProcPointer;

type
  WICColorContextType = type Integer;
const
  WICColorContextUninitialized  = 0;
  WICColorContextProfile        = $1;
  WICColorContextExifColorSpace = $2;

type
  REFWICPixelFormatGUID = PGUID;
  WICPixelFormatGUID = TGUID;
  TWICPixelFormatGUID = WICPixelFormatGUID;
  PWICPixelFormatGUID = ^TWICPixelFormatGUID;

const
  GUID_WICPixelFormatUndefined:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC900}';
  GUID_WICPixelFormatDontCare:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC900}';
  GUID_WICPixelFormat1bppIndexed:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC901}';
  GUID_WICPixelFormat2bppIndexed:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC902}';
  GUID_WICPixelFormat4bppIndexed:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC903}';
  GUID_WICPixelFormat8bppIndexed:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC904}';
  GUID_WICPixelFormatBlackWhite:           TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC905}';
  GUID_WICPixelFormat2bppGray:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC906}';
  GUID_WICPixelFormat4bppGray:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC907}';
  GUID_WICPixelFormat8bppGray:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC908}';
  GUID_WICPixelFormat8bppAlpha:            TGUID = '{E6CD0116-EEBA-4161-AA85-27DD9FB3A895}';
  GUID_WICPixelFormat16bppBGR555:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC909}';
  GUID_WICPixelFormat16bppBGR565:          TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90A}';
  GUID_WICPixelFormat16bppBGRA5551:        TGUID = '{05EC7C2B-F1E6-4961-AD46-E1CC810A87D2}';
  GUID_WICPixelFormat16bppGray:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90B}';
  GUID_WICPixelFormat24bppBGR:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90C}';
  GUID_WICPixelFormat24bppRGB:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90D}';
  GUID_WICPixelFormat32bppBGR:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90E}';
  GUID_WICPixelFormat32bppBGRA:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC90F}';
  GUID_WICPixelFormat32bppPBGRA:           TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC910}';
  GUID_WICPixelFormat32bppGrayFloat:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC911}';
  GUID_WICPixelFormat32bppRGBA:            TGUID = '{F5C7AD2D-6A8D-43DD-A7A8-A29935261AE9}';
  GUID_WICPixelFormat32bppPRGBA:           TGUID = '{3CC4A650-A527-4D37-A916-3142C7EBEDBA}';
  GUID_WICPixelFormat48bppRGB:             TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC915}';
  GUID_WICPixelFormat48bppBGR:             TGUID = '{E605A384-B468-46CE-BB2E-36F180E64313}';
  GUID_WICPixelFormat64bppRGBA:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC916}';
  GUID_WICPixelFormat64bppBGRA:            TGUID = '{1562FF7C-D352-46F9-979E-42976B792246}';
  GUID_WICPixelFormat64bppPRGBA:           TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC917}';
  GUID_WICPixelFormat64bppPBGRA:           TGUID = '{8C518E8E-A4EC-468B-AE70-C9A35A9C5530}';
  GUID_WICPixelFormat16bppGrayFixedPoint:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC913}';
  GUID_WICPixelFormat32bppBGR101010:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC914}';
  GUID_WICPixelFormat48bppRGBFixedPoint:   TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC912}';
  GUID_WICPixelFormat48bppBGRFixedPoint:   TGUID = '{49CA140E-CAB6-493B-9DDF-60187C37532A}';
  GUID_WICPixelFormat96bppRGBFixedPoint:   TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC918}';
  GUID_WICPixelFormat128bppRGBAFloat:      TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC919}';
  GUID_WICPixelFormat128bppPRGBAFloat:     TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91A}';
  GUID_WICPixelFormat128bppRGBFloat:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91B}';
  GUID_WICPixelFormat32bppCMYK:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91C}';
  GUID_WICPixelFormat64bppRGBAFixedPoint:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91D}';
  GUID_WICPixelFormat64bppBGRAFixedPoint:  TGUID = '{356de33c-54d2-4a23-bb04-9b7bf9b1d42d}';
  GUID_WICPixelFormat64bppRGBFixedPoint:   TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC940}';
  GUID_WICPixelFormat128bppRGBAFixedPoint: TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91E}';
  GUID_WICPixelFormat128bppRGBFixedPoint:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC941}';
  GUID_WICPixelFormat64bppRGBAHalf:        TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC93A}';
  GUID_WICPixelFormat64bppRGBHalf:         TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC942}';
  GUID_WICPixelFormat48bppRGBHalf:         TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC93B}';
  GUID_WICPixelFormat32bppRGBE:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC93D}';
  GUID_WICPixelFormat16bppGrayHalf:        TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC93E}';
  GUID_WICPixelFormat32bppGrayFixedPoint:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC93F}';
  GUID_WICPixelFormat32bppRGBA1010102:     TGUID = '{25238D72-FCF9-4522-B514-5578E5AD55E0}';
  GUID_WICPixelFormat32bppRGBA1010102XR:   TGUID = '{00DE6B9A-C101-434B-B502-D0165EE1122C}';
  GUID_WICPixelFormat64bppCMYK:            TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC91F}';
  GUID_WICPixelFormat24bpp3Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC920}';
  GUID_WICPixelFormat32bpp4Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC921}';
  GUID_WICPixelFormat40bpp5Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC922}';
  GUID_WICPixelFormat48bpp6Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC923}';
  GUID_WICPixelFormat56bpp7Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC924}';
  GUID_WICPixelFormat64bpp8Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC925}';
  GUID_WICPixelFormat48bpp3Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC926}';
  GUID_WICPixelFormat64bpp4Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC927}';
  GUID_WICPixelFormat80bpp5Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC928}';
  GUID_WICPixelFormat96bpp6Channels:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC929}';
  GUID_WICPixelFormat112bpp7Channels:      TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92A}';
  GUID_WICPixelFormat128bpp8Channels:      TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92B}';
  GUID_WICPixelFormat40bppCMYKAlpha:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92C}';
  GUID_WICPixelFormat80bppCMYKAlpha:       TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92D}';
  GUID_WICPixelFormat32bpp3ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92E}';
  GUID_WICPixelFormat40bpp4ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC92F}';
  GUID_WICPixelFormat48bpp5ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC930}';
  GUID_WICPixelFormat56bpp6ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC931}';
  GUID_WICPixelFormat64bpp7ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC932}';
  GUID_WICPixelFormat72bpp8ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC933}';
  GUID_WICPixelFormat64bpp3ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC934}';
  GUID_WICPixelFormat80bpp4ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC935}';
  GUID_WICPixelFormat96bpp5ChannelsAlpha:  TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC936}';
  GUID_WICPixelFormat112bpp6ChannelsAlpha: TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC937}';
  GUID_WICPixelFormat128bpp7ChannelsAlpha: TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC938}';
  GUID_WICPixelFormat144bpp8ChannelsAlpha: TGUID = '{6FDDC324-4E03-4BFE-B185-3D77768DC939}';
type
  WICBitmapCreateCacheOption = type Integer;
const
  WICBitmapNoCache                       = 0;
  WICBitmapCacheOnDemand                 = $1;
  WICBitmapCacheOnLoad                   = $2;
  WICBITMAPCREATECACHEOPTION_FORCE_DWORD = $7FFFFFFF;

type
  WICDecodeOptions = type Integer;
const
  WICDecodeMetadataCacheOnDemand     = 0;
  WICDecodeMetadataCacheOnLoad       = $1;
  WICMETADATACACHEOPTION_FORCE_DWORD = $7FFFFFFF;

type
  WICBitmapEncoderCacheOption = type Integer;
const
  WICBitmapEncoderCacheInMemory           = 0;
  WICBitmapEncoderCacheTempFile           = $1;
  WICBitmapEncoderNoCache                 = $2;
  WICBITMAPENCODERCACHEOPTION_FORCE_DWORD = $7FFFFFFF;

type
  WICComponentType = type Integer;
const
  WICDecoder                   = $1;
  WICEncoder                   = $2;
  WICPixelFormatConverter      = $4;
  WICMetadataReader            = $8;
  WICMetadataWriter            = $10;
  WICPixelFormat               = $20;
  WICAllComponents             = $3F;
  WICCOMPONENTTYPE_FORCE_DWORD = $7FFFFFFF;

type
  WICComponentEnumerateOptions = type Integer;
const
  WICComponentEnumerateDefault             = 0;
  WICComponentEnumerateRefresh             = $1;
  WICComponentEnumerateDisabled            = $80000000;
  WICComponentEnumerateUnsigned            = $40000000;
  WICComponentEnumerateBuiltInOnly         = $20000000;
  WICCOMPONENTENUMERATEOPTIONS_FORCE_DWORD = $7FFFFFFF;

type
  WICBitmapPattern = record
    Position: ULARGE_INTEGER;
    Length: ULONG;
    Pattern: PBYTE;
    Mask: PBYTE;
    EndOfStream: BOOL;
  end;
  TWICBitmapPattern = WICBitmapPattern;
  PWICBitmapPattern = ^WICBitmapPattern;

type
  WICBitmapInterpolationMode = type Integer;
const
  WICBitmapInterpolationModeNearestNeighbor = 0;
  WICBitmapInterpolationModeLinear          = $1;
  WICBitmapInterpolationModeCubic           = $2;
  WICBitmapInterpolationModeFant            = $3;
  WICBITMAPINTERPOLATIONMODE_FORCE_DWORD    = $7FFFFFFF;

type
  WICBitmapPaletteType = type Integer;
const
  WICBitmapPaletteTypeCustom           = 0;
  WICBitmapPaletteTypeMedianCut        = $1;
  WICBitmapPaletteTypeFixedBW          = $2;
  WICBitmapPaletteTypeFixedHalftone8   = $3;
  WICBitmapPaletteTypeFixedHalftone27  = $4;
  WICBitmapPaletteTypeFixedHalftone64  = $5;
  WICBitmapPaletteTypeFixedHalftone125 = $6;
  WICBitmapPaletteTypeFixedHalftone216 = $7;
  WICBitmapPaletteTypeFixedWebPalette  = WICBITMAPPALETTETYPEFIXEDHALFTONE216;
  WICBitmapPaletteTypeFixedHalftone252 = $8;
  WICBitmapPaletteTypeFixedHalftone256 = $9;
  WICBitmapPaletteTypeFixedGray4       = $A;
  WICBitmapPaletteTypeFixedGray16      = $B;
  WICBitmapPaletteTypeFixedGray256     = $C;
  WICBITMAPPALETTETYPE_FORCE_DWORD     = $7FFFFFFF;

type
  WICBitmapDitherType = type Integer;
const
  WICBitmapDitherTypeNone           = 0;
  WICBitmapDitherTypeSolid          = 0;
  WICBitmapDitherTypeOrdered4x4     = $1;
  WICBitmapDitherTypeOrdered8x8     = $2;
  WICBitmapDitherTypeOrdered16x16   = $3;
  WICBitmapDitherTypeSpiral4x4      = $4;
  WICBitmapDitherTypeSpiral8x8      = $5;
  WICBitmapDitherTypeDualSpiral4x4  = $6;
  WICBitmapDitherTypeDualSpiral8x8  = $7;
  WICBitmapDitherTypeErrorDiffusion = $8;
  WICBITMAPDITHERTYPE_FORCE_DWORD   = $7FFFFFFF;

type
  WICBitmapAlphaChannelOption = type Integer;
const
  WICBitmapUseAlpha                        = 0;
  WICBitmapUsePremultipliedAlpha           = $1;
  WICBitmapIgnoreAlpha                     = $2;
  WICBITMAPALPHACHANNELOPTIONS_FORCE_DWORD = $7FFFFFFF;

type
  WICBitmapTransformOptions = type Integer;
const
  WICBitmapTransformRotate0             = 0;
  WICBitmapTransformRotate90            = $1;
  WICBitmapTransformRotate180           = $2;
  WICBitmapTransformRotate270           = $3;
  WICBitmapTransformFlipHorizontal      = $8;
  WICBitmapTransformFlipVertical        = $10;
  WICBITMAPTRANSFORMOPTIONS_FORCE_DWORD = $7FFFFFFF;

type
  WICBitmapLockFlags = type Integer;
const
  WICBitmapLockRead              = $1;
  WICBitmapLockWrite             = $2;
  WICBITMAPLOCKFLAGS_FORCE_DWORD = $7FFFFFFF;

type
  WICBitmapDecoderCapabilities = type Integer;
const
  WICBitmapDecoderCapabilitySameEncoder          = $1;
  WICBitmapDecoderCapabilityCanDecodeAllImages   = $2;
  WICBitmapDecoderCapabilityCanDecodeSomeImages  = $4;
  WICBitmapDecoderCapabilityCanEnumerateMetadata = $8;
  WICBitmapDecoderCapabilityCanDecodeThumbnail   = $10;
  WICBITMAPDECODERCAPABILITIES_FORCE_DWORD       = $7FFFFFFF;

type
  WICProgressOperation = type Integer;
const
  WICProgressOperationCopyPixels   = $1;
  WICProgressOperationWritePixels  = $2;
  WICProgressOperationAll          = $FFFF;
  WICPROGRESSOPERATION_FORCE_DWORD = $7FFFFFFF;

type
  WICProgressNotification = type Integer;
const
  WICProgressNotificationBegin        = $10000;
  WICProgressNotificationEnd          = $20000;
  WICProgressNotificationFrequent     = $40000;
  WICProgressNotificationAll          = $FFFF0000;
  WICPROGRESSNOTIFICATION_FORCE_DWORD = $7FFFFFFF;

type
  WICComponentSigning = type Integer;
const
  WICComponentSigned              = $1;
  WICComponentUnsigned            = $2;
  WICComponentSafe                = $4;
  WICComponentDisabled            = $80000000;
  WICCOMPONENTSIGNING_FORCE_DWORD = $7FFFFFFF;

type
  WICGifLogicalScreenDescriptorProperties = type Integer;
const
  WICGifLogicalScreenSignature                        = $1;
  WICGifLogicalScreenDescriptorWidth                  = $2;
  WICGifLogicalScreenDescriptorHeight                 = $3;
  WICGifLogicalScreenDescriptorGlobalColorTableFlag   = $4;
  WICGifLogicalScreenDescriptorColorResolution        = $5;
  WICGifLogicalScreenDescriptorSortFlag               = $6;
  WICGifLogicalScreenDescriptorGlobalColorTableSize   = $7;
  WICGifLogicalScreenDescriptorBackgroundColorIndex   = $8;
  WICGifLogicalScreenDescriptorPixelAspectRatio       = $9;
  WICGifLogicalScreenDescriptorProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICGifImageDescriptorProperties = type Integer;
const
  WICGifImageDescriptorLeft                   = $1;
  WICGifImageDescriptorTop                    = $2;
  WICGifImageDescriptorWidth                  = $3;
  WICGifImageDescriptorHeight                 = $4;
  WICGifImageDescriptorLocalColorTableFlag    = $5;
  WICGifImageDescriptorInterlaceFlag          = $6;
  WICGifImageDescriptorSortFlag               = $7;
  WICGifImageDescriptorLocalColorTableSize    = $8;
  WICGifImageDescriptorProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICGifGraphicControlExtensionProperties = type Integer;
const
  WICGifGraphicControlExtensionDisposal               = $1;
  WICGifGraphicControlExtensionUserInputFlag          = $2;
  WICGifGraphicControlExtensionTransparencyFlag       = $3;
  WICGifGraphicControlExtensionDelay                  = $4;
  WICGifGraphicControlExtensionTransparentColorIndex  = $5;
  WICGifGraphicControlExtensionProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICGifApplicationExtensionProperties = type Integer;
const
  WICGifApplicationExtensionApplication            = $1;
  WICGifApplicationExtensionData                   = $2;
  WICGifApplicationExtensionProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICGifCommentExtensionProperties = type Integer;
const
  WICGifCommentExtensionText                   = $1;
  WICGifCommentExtensionProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICJpegCommentProperties = type Integer;
const
  WICJpegCommentText                   = $1;
  WICJpegCommentProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICJpegLuminanceProperties = type Integer;
const
  WICJpegLuminanceTable                  = $1;
  WICJpegLuminanceProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICJpegChrominanceProperties = type Integer;
const
  WICJpegChrominanceTable                  = $1;
  WICJpegChrominanceProperties_FORCE_DWORD = $7FFFFFFF;

type
  WIC8BIMIptcProperties = type Integer;
const
  WIC8BIMIptcPString                = 0;
  WIC8BIMIptcEmbeddedIPTC           = $1;
  WIC8BIMIptcProperties_FORCE_DWORD = $7FFFFFFF;

type
  WIC8BIMResolutionInfoProperties = type Integer;
const
  WIC8BIMResolutionInfoPString                = $1;
  WIC8BIMResolutionInfoHResolution            = $2;
  WIC8BIMResolutionInfoHResolutionUnit        = $3;
  WIC8BIMResolutionInfoWidthUnit              = $4;
  WIC8BIMResolutionInfoVResolution            = $5;
  WIC8BIMResolutionInfoVResolutionUnit        = $6;
  WIC8BIMResolutionInfoHeightUnit             = $7;
  WIC8BIMResolutionInfoProperties_FORCE_DWORD = $7FFFFFFF;

type
  WIC8BIMIptcDigestProperties = type Integer;
const
  WIC8BIMIptcDigestPString                = $1;
  WIC8BIMIptcDigestIptcDigest             = $2;
  WIC8BIMIptcDigestProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngGamaProperties = type Integer;
const
  WICPngGamaGamma                  = $1;
  WICPngGamaProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngBkgdProperties = type Integer;
const
  WICPngBkgdBackgroundColor        = $1;
  WICPngBkgdProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngItxtProperties = type Integer;
const
  WICPngItxtKeyword                = $1;
  WICPngItxtCompressionFlag        = $2;
  WICPngItxtLanguageTag            = $3;
  WICPngItxtTranslatedKeyword      = $4;
  WICPngItxtText                   = $5;
  WICPngItxtProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngChrmProperties = type Integer;
const
  WICPngChrmWhitePointX            = $1;
  WICPngChrmWhitePointY            = $2;
  WICPngChrmRedX                   = $3;
  WICPngChrmRedY                   = $4;
  WICPngChrmGreenX                 = $5;
  WICPngChrmGreenY                 = $6;
  WICPngChrmBlueX                  = $7;
  WICPngChrmBlueY                  = $8;
  WICPngChrmProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngHistProperties = type Integer;
const
  WICPngHistFrequencies            = $1;
  WICPngHistProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngIccpProperties = type Integer;
const
  WICPngIccpProfileName            = $1;
  WICPngIccpProfileData            = $2;
  WICPngIccpProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngSrgbProperties = type Integer;
const
  WICPngSrgbRenderingIntent        = $1;
  WICPngSrgbProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICPngTimeProperties = type Integer;
const
  WICPngTimeYear                   = $1;
  WICPngTimeMonth                  = $2;
  WICPngTimeDay                    = $3;
  WICPngTimeHour                   = $4;
  WICPngTimeMinute                 = $5;
  WICPngTimeSecond                 = $6;
  WICPngTimeProperties_FORCE_DWORD = $7FFFFFFF;

type
  WICSectionAccessLevel = type Integer;
const
  WICSectionAccessLevelRead         = $1;
  WICSectionAccessLevelReadWrite    = $3;
  WICSectionAccessLevel_FORCE_DWORD = $7FFFFFFF;

type
  WICPixelFormatNumericRepresentation = type Integer;
const
  WICPixelFormatNumericRepresentationUnspecified     = 0;
  WICPixelFormatNumericRepresentationIndexed         = $1;
  WICPixelFormatNumericRepresentationUnsignedInteger = $2;
  WICPixelFormatNumericRepresentationSignedInteger   = $3;
  WICPixelFormatNumericRepresentationFixed           = $4;
  WICPixelFormatNumericRepresentationFloat           = $5;
  WICPixelFormatNumericRepresentation_FORCE_DWORD    = $7FFFFFFF;

type
  IWICBitmapSource = interface;
  IWICMetadataQueryWriter = interface;
  IWICBitmapEncoderInfo = interface;
  IWICBitmapFrameEncode = interface;
  IWICBitmapDecoderInfo = interface;
  IWICBitmapFrameDecode = interface;

{ interface IWICPalette }
  IWICPalette = interface(IUnknown)
    [SID_IWICPalette]
    function InitializePredefined(ePaletteType: WICBitmapPaletteType;fAddTransparentColor: BOOL): HRESULT; stdcall;
    function InitializeCustom(pColors: PWICColor; cCount: UINT): HRESULT; stdcall;
    function InitializeFromBitmap(pISurface: IWICBitmapSource; cCount: UINT; fAddTransparentColor: BOOL): HRESULT; stdcall;
    function InitializeFromPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function GetType(var pePaletteType: WICBitmapPaletteType): HRESULT; stdcall;
    function GetColorCount(var pcCount: UINT): HRESULT; stdcall;
    function GetColors(cCount: UINT; pColors: PWICColor; var pcActualColors: UINT): HRESULT; stdcall;
    function IsBlackWhite(var pfIsBlackWhite: BOOL): HRESULT; stdcall;
    function IsGrayscale(var pfIsGrayscale: BOOL): HRESULT; stdcall;
    function HasAlpha(var pfHasAlpha: BOOL): HRESULT; stdcall;
  end;

{ interface IWICBitmapSource }
  IWICBitmapSource = interface(IUnknown)
    [SID_IWICBitmapSource]
    function GetSize(var puiWidth: UINT; var puiHeight: UINT): HRESULT; stdcall;
    function GetPixelFormat(var pPixelFormat: WICPixelFormatGUID): HRESULT; stdcall;
    function GetResolution(var pDpiX: Double; var pDpiY: Double): HRESULT; stdcall;
    function CopyPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function CopyPixels(prc: PWICRect; cbStride: UINT; cbBufferSize: UINT; pbBuffer: PByte): HRESULT; stdcall;
  end;

{ interface IWICFormatConverter }
  IWICFormatConverter = interface(IWICBitmapSource)
    [SID_IWICFormatConverter]
    function Initialize(pISource: IWICBitmapSource;
                         const dstFormat: WICPixelFormatGUID; dither: WICBitmapDitherType;
                         const pIPalette: IWICPalette; alphaThresholdPercent: Double;
                         paletteTranslate: WICBitmapPaletteType): HRESULT; stdcall;
    function CanConvert(srcPixelFormat: REFWICPixelFormatGUID; dstPixelFormat: REFWICPixelFormatGUID;var pfCanConvert: BOOL): HRESULT; stdcall;
  end;

{ interface IWICBitmapScaler }
  IWICBitmapScaler = interface(IWICBitmapSource)
    [SID_IWICBitmapScaler]
    function Initialize(pISource: IWICBitmapSource; uiWidth: UINT; uiHeight: UINT; mode: WICBitmapInterpolationMode): HRESULT; stdcall;
  end;

{ interface IWICBitmapClipper }
  IWICBitmapClipper = interface(IWICBitmapSource)
    [SID_IWICBitmapClipper]
    function Initialize(pISource: IWICBitmapSource; var prc: WICRect): HRESULT; stdcall;
  end;

{ interface IWICBitmapFlipRotator }
  IWICBitmapFlipRotator = interface(IWICBitmapSource)
    [SID_IWICBitmapFlipRotator]
    function Initialize(pISource: IWICBitmapSource; options: WICBitmapTransformOptions): HRESULT; stdcall;
  end;

{ interface IWICBitmapLock }
  IWICBitmapLock = interface(IUnknown)
    [SID_IWICBitmapLock]
    function GetSize(var puiWidth: UINT; var puiHeight: UINT): HRESULT; stdcall;
    function GetStride(var pcbStride: UINT): HRESULT; stdcall;
    function GetDataPointer(var pcbBufferSize: UINT; var ppbData: WICInProcPointer): HRESULT; stdcall;
    function GetPixelFormat(var pPixelFormat: WICPixelFormatGUID): HRESULT; stdcall;
  end;

{ interface IWICBitmap }
  IWICBitmap = interface(IWICBitmapSource)
    [SID_IWICBitmap]
    function Lock(const prcLock: WICRect; flags: DWORD; out ppILock: IWICBitmapLock): HRESULT; stdcall;
    function SetPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function SetResolution(dpiX: Double; dpiY: Double): HRESULT; stdcall;
  end;

{ interface IWICColorContext }
  IWICColorContext = interface(IUnknown)
    [SID_IWICColorContext]
    function InitializeFromFilename(wzFilename: LPCWSTR): HRESULT; stdcall;
    function InitializeFromMemory(const pbBuffer: PByte; cbBufferSize: UINT): HRESULT; stdcall;
    function InitializeFromExifColorSpace(value: UINT): HRESULT; stdcall;
    function GetType(var pType: WICColorContextType): HRESULT; stdcall;
    function GetProfileBytes(cbBuffer: UINT; pbBuffer: PBYTE; var pcbActual: UINT): HRESULT; stdcall;
    function GetExifColorSpace(var pValue: UINT): HRESULT; stdcall;
  end;
  PIWICColorContext = ^IWICColorContext;

{ interface IWICColorTransform }
  IWICColorTransform = interface(IWICBitmapSource)
    [SID_IWICColorTransform]
    function Initialize(pIBitmapSource: IWICBitmapSource;
                         pIContextSource: IWICColorContext; pIContextDest: IWICColorContext;
                         pixelFmtDest: REFWICPixelFormatGUID): HRESULT; stdcall;
  end;

{ interface IWICFastMetadataEncoder }
  IWICFastMetadataEncoder = interface(IUnknown)
    [SID_IWICFastMetadataEncoder]
    function Commit: HRESULT; stdcall;
    function GetMetadataQueryWriter(out ppIMetadataQueryWriter: IWICMetadataQueryWriter): HRESULT; stdcall;
  end;

{ interface IWICStream }
  IWICStream = interface(IStream)
    [SID_IWICStream]
    function InitializeFromIStream(pIStream: IStream): HRESULT; stdcall;
    function InitializeFromFilename(wzFileName: LPCWSTR; dwDesiredAccess: DWORD): HRESULT; stdcall;
    function InitializeFromMemory(pbBuffer: WICInProcPointer; cbBufferSize: DWORD): HRESULT; stdcall;
    function InitializeFromIStreamRegion(pIStream: IStream; ulOffset: ULARGE_INTEGER; ulMaxSize: ULARGE_INTEGER): HRESULT; stdcall;
  end;

{ interface IWICEnumMetadataItem }
  IWICEnumMetadataItem = interface(IUnknown)
    [SID_IWICEnumMetadataItem]
    function Next(celt: Cardinal;
                   rgeltSchema: PPropVariant;
                   rgeltID: PPropVariant;
                   rgeltValue: PPropVariant;
                   var pceltFetched: ULONG): HRESULT; stdcall;
    function Skip(celt: Cardinal): HRESULT; stdcall;
    function Reset: HRESULT; stdcall;
    function Clone(out ppIEnumMetadataItem: IWICEnumMetadataItem): HRESULT; stdcall;
  end;

{ interface IWICMetadataQueryReader }
  IWICMetadataQueryReader = interface(IUnknown)
    [SID_IWICMetadataQueryReader]
    function GetContainerFormat(var pguidContainerFormat: TGUID): HRESULT; stdcall;
    function GetLocation(cchMaxLength: UINT; wzNamespace: PWCHAR; var pcchActualLength: UINT): HRESULT; stdcall;
    function GetMetadataByName(wzName: LPCWSTR; var pvarValue: PROPVARIANT): HRESULT; stdcall;
    function GetEnumerator(out ppIEnumString: IEnumString): HRESULT; stdcall;
  end;

{ interface IWICMetadataQueryWriter }
  IWICMetadataQueryWriter = interface(IWICMetadataQueryReader)
    [SID_IWICMetadataQueryWriter]
    function SetMetadataByName(wzName: LPCWSTR; const pvarValue: TPropVariant): HRESULT; stdcall;
    function RemoveMetadataByName(wzName: LPCWSTR): HRESULT; stdcall;
  end;

{ interface IWICBitmapEncoder }
  IWICBitmapEncoder = interface(IUnknown)
    [SID_IWICBitmapEncoder]
    function Initialize(pIStream: IStream; cacheOption: WICBitmapEncoderCacheOption): HRESULT; stdcall;
    function GetContainerFormat(var pguidContainerFormat: TGUID): HRESULT; stdcall;
    function GetEncoderInfo(out ppIEncoderInfo: IWICBitmapEncoderInfo): HRESULT; stdcall;
    function SetColorContexts(cCount: UINT; ppIColorContext: PIWICColorContext): HRESULT; stdcall;
    function SetPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function SetThumbnail(pIThumbnail: IWICBitmapSource): HRESULT; stdcall;
    function SetPreview(pIPreview: IWICBitmapSource): HRESULT; stdcall;
    function CreateNewFrame(out ppIFrameEncode: IWICBitmapFrameEncode; var ppIEncoderOptions: IPropertyBag2): HRESULT; stdcall;
    function Commit: HRESULT; stdcall;
    function GetMetadataQueryWriter(out ppIMetadataQueryWriter: IWICMetadataQueryWriter): HRESULT; stdcall;
  end;

{ interface IWICBitmapFrameEncode }
  IWICBitmapFrameEncode = interface(IUnknown)
    [SID_IWICBitmapFrameEncode]
    function Initialize(pIEncoderOptions: IUnknown): HRESULT; stdcall;
    function SetSize(uiWidth: UINT; uiHeight: UINT): HRESULT; stdcall;
    function SetResolution(dpiX: Double; dpiY: Double): HRESULT; stdcall;
    function SetPixelFormat(var pPixelFormat: WICPixelFormatGUID): HRESULT; stdcall;
    function SetColorContexts(cCount: UINT; ppIColorContext: PIWICColorContext): HRESULT; stdcall;
    function SetPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function SetThumbnail(pIThumbnail: IWICBitmapSource): HRESULT; stdcall;
    function WritePixels(lineCount: UINT; cbStride: UINT; cbBufferSize: UINT; pbPixels: PByte): HRESULT; stdcall;
    function WriteSource(pIBitmapSource: IWICBitmapSource; prc: PWICRect): HRESULT; stdcall;
    function Commit: HRESULT; stdcall;
    function GetMetadataQueryWriter(out ppIMetadataQueryWriter: IWICMetadataQueryWriter): HRESULT; stdcall;
  end;

{ interface IWICBitmapDecoder }
  IWICBitmapDecoder = interface(IUnknown)
    [SID_IWICBitmapDecoder]
    function QueryCapability(pIStream: IStream; var pdwCapability: DWORD): HRESULT; stdcall;
    function Initialize(pIStream: IStream; cacheOptions: WICDecodeOptions): HRESULT; stdcall;
    function GetContainerFormat(var pguidContainerFormat: TGUID): HRESULT; stdcall;
    function GetDecoderInfo(out ppIDecoderInfo: IWICBitmapDecoderInfo): HRESULT; stdcall;
    function CopyPalette(pIPalette: IWICPalette): HRESULT; stdcall;
    function GetMetadataQueryReader(out ppIMetadataQueryReader: IWICMetadataQueryReader): HRESULT; stdcall;
    function GetPreview(out ppIBitmapSource: IWICBitmapSource): HRESULT; stdcall;
    function GetColorContexts(cCount: UINT; ppIColorContexts: PIWICColorContext; var pcActualCount : UINT): HRESULT; stdcall;
    function GetThumbnail(out ppIThumbnail: IWICBitmapSource): HRESULT; stdcall;
    function GetFrameCount(var pCount: UINT): HRESULT; stdcall;
    function GetFrame(index: UINT; out ppIBitmapFrame: IWICBitmapFrameDecode): HRESULT; stdcall;

  end;

{ interface IWICBitmapSourceTransform }
  IWICBitmapSourceTransform = interface(IUnknown)
    [SID_IWICBitmapSourceTransform]
    function CopyPixels(var prc: WICRect; uiWidth: UINT; uiHeight: UINT;
                         var pguidDstFormat: WICPixelFormatGUID;
                         dstTransform: WICBitmapTransformOptions; nStride: UINT; cbBufferSize: UINT;
                         pbBuffer: PByte): HRESULT; stdcall;
    function GetClosestSize(var puiWidth: UINT; var puiHeight: UINT): HRESULT; stdcall;
    function GetClosestPixelFormat(var pguidDstFormat: WICPixelFormatGUID): HRESULT; stdcall;
    function DoesSupportTransform(dstTransform: WICBitmapTransformOptions; var pfIsSupported: BOOL): HRESULT; stdcall;
  end;

{ interface IWICBitmapFrameDecode }
  IWICBitmapFrameDecode = interface(IWICBitmapSource)
    [SID_IWICBitmapFrameDecode]
    function GetMetadataQueryReader(out ppIMetadataQueryReader: IWICMetadataQueryReader): HRESULT; stdcall;
    function GetColorContexts(cCount: UINT; ppIColorContexts: PIWICColorContext; var pcActualCount : UINT): HRESULT; stdcall;
    function GetThumbnail(out ppIThumbnail: IWICBitmapSource): HRESULT; stdcall;
  end;

{ interface IWICProgressiveLevelControl }
  IWICProgressiveLevelControl = interface(IUnknown)
    [SID_IWICProgressiveLevelControl]
    function GetLevelCount(var pcLevels: UINT): HRESULT; stdcall;
    function GetCurrentLevel(var pnLevel: UINT): HRESULT; stdcall;
    function SetCurrentLevel(nLevel: UINT): HRESULT; stdcall;
  end;

{ interface IWICProgressCallback }
  IWICProgressCallback = interface(IUnknown)
    [SID_IWICProgressCallback]
    function Notify(uFrameNum: Cardinal; operation: WICProgressOperation; dblProgress: Double): HRESULT; stdcall;
  end;

{ interface IWICBitmapCodecProgressNotification }
  FNProgressNotification = function(pvData: Pointer; uFrameNum: Cardinal;
    operation: WICProgressOperation; dblProgress: Double): HRESULT; stdcall;
  PFNProgressNotification = ^FNProgressNotification;

  IWICBitmapCodecProgressNotification = interface(IUnknown)
    [SID_IWICBitmapCodecProgressNotification]
    function RegisterProgressNotification( pfnProgressNotification: PFNProgressNotification; pvData: Pointer; dwProgressFlags: DWORD): HRESULT; stdcall;
  end;

{ interface IWICComponentInfo }
  IWICComponentInfo = interface(IUnknown)
    [SID_IWICComponentInfo]
    function GetComponentType(var pType: WICComponentType): HRESULT; stdcall;
    function GetCLSID(var pclsid: TGUID): HRESULT; stdcall;
    function GetSigningStatus(var pStatus: DWORD): HRESULT; stdcall;
    function GetAuthor(cchAuthor: UINT; wzAuthor: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetVendorGUID(var pguidVendor: TGUID): HRESULT; stdcall;
    function GetVersion(cchVersion: UINT; wzVersion: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetSpecVersion(cchSpecVersion: UINT; wzSpecVersion: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetFriendlyName(cchFriendlyName: UINT; wzFriendlyName: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
  end;

{ interface IWICFormatConverterInfo }
  IWICFormatConverterInfo = interface(IWICComponentInfo)
    [SID_IWICFormatConverterInfo]
    function GetPixelFormats(cFormats: UINT; pPixleFormatGUIDs: PWICPixelFormatGUID; var pcActual: UINT): HRESULT; stdcall;
    function CreateInstance(out ppIConverter: IWICFormatConverter): HRESULT; stdcall;
  end;

{ interface IWICBitmapCodecInfo }
  IWICBitmapCodecInfo = interface(IWICComponentInfo)
    [SID_IWICBitmapCodecInfo]
    function GetContainerFormat(var pguidContainerFormat: TGUID): HRESULT; stdcall;
    function GetPixelFormats(cFormats: UINT; var guidPixelFormats: PGUID; var pcActual: UINT): HRESULT; stdcall;
    function GetColorManagementVersion(cchColorManagementVersion: UINT; wzColorManagementVersion: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetDeviceManufacturer(cchDeviceManufacturer: UINT; wzDeviceManufacturer: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetDeviceModels(cchDeviceModels: UINT; wzDeviceModels: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetMimeTypes(cchMimeTypes: UINT; wzMimeTypes: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function GetFileExtensions(cchFileExtensions: UINT; wzFileExtensions: PWCHAR; var pcchActual: UINT): HRESULT; stdcall;
    function DoesSupportAnimation(var pfSupportAnimation: BOOL): HRESULT; stdcall;
    function DoesSupportChromakey(var pfSupportChromakey: BOOL): HRESULT; stdcall;
    function DoesSupportLossless(var pfSupportLossless: BOOL): HRESULT; stdcall;
    function DoesSupportMultiframe(var pfSupportMultiframe: BOOL): HRESULT; stdcall;
    function MatchesMimeType(wzMimeType: LPCWSTR;var pfMatches: BOOL): HRESULT; stdcall;

  end;

{ interface IWICBitmapEncoderInfo }
  IWICBitmapEncoderInfo = interface(IWICBitmapCodecInfo)
    [SID_IWICBitmapEncoderInfo]
    function CreateInstance(out ppIBitmapEncoder: IWICBitmapEncoder): HRESULT; stdcall;
  end;

{ interface IWICBitmapDecoderInfo }
  IWICBitmapDecoderInfo = interface(IWICBitmapCodecInfo)
    [SID_IWICBitmapDecoderInfo]
    function GetPatterns(cbSizePatterns: UINT; pPatterns: PWICBitmapPattern;
                          var pcPatterns: UINT; var pcbPatternsActual: UINT): HRESULT; stdcall;
    function MatchesPattern(pIStream: IStream;var pfMatches: BOOL): HRESULT; stdcall;
    function CreateInstance(out ppIBitmapDecoder: IWICBitmapDecoder): HRESULT; stdcall;
  end;

{ interface IWICPixelFormatInfo }
  IWICPixelFormatInfo = interface(IWICComponentInfo)
    [SID_IWICPixelFormatInfo]
    function GetFormatGUID(var pFormat: TGUID): HRESULT; stdcall;
    function GetColorContext(out ppIColorContext: IWICColorContext): HRESULT; stdcall;
    function GetBitsPerPixel(var puiBitsPerPixel: UINT): HRESULT; stdcall;
    function GetChannelCount(var puiChannelCount: UINT): HRESULT; stdcall;
    function GetChannelMask(uiChannelIndex: UINT; cbMaskBuffer: UINT;
                             pbMaskBuffer: PBYTE; var pcbActual: UINT): HRESULT; stdcall;
  end;

{ interface IWICPixelFormatInfo2 }
  IWICPixelFormatInfo2 = interface(IWICPixelFormatInfo)
    [SID_IWICPixelFormatInfo2]
    function SupportsTransparency(var pfSupportsTransparency: BOOL): HRESULT; stdcall;
    function GetNumericRepresentation(var pNumericRepresentation: WICPixelFormatNumericRepresentation): HRESULT; stdcall;
  end;

{ interface IWICImagingFactory }
  IWICImagingFactory = interface(IUnknown)
    [SID_IWICImagingFactory]
    function CreateDecoderFromFilename(wzFilename: LPCWSTR; pguidVendor: PGUID;
      dwDesiredAccess: DWORD; metadataOptions: WICDecodeOptions;
      out ppIDecoder: IWICBitmapDecoder): HRESULT; stdcall;

    function CreateDecoderFromStream(pIStream: IStream; pguidVendor: PGUID;
      metadataOptions: WICDecodeOptions;
      out ppIDecoder: IWICBitmapDecoder): HRESULT; stdcall;

    function CreateDecoderFromFileHandle(hFile: LongWord; const pguidVendor: TGUID;
      metadataOptions: WICDecodeOptions;
      out ppIDecoder: IWICBitmapDecoder): HRESULT; stdcall;

    function CreateComponentInfo(const clsidComponent: TGUID;
      out ppIInfo: IWICComponentInfo): HRESULT; stdcall;

    function CreateDecoder(const guidContainerFormat: TGuid; const pguidVendor: TGUID;
      out ppIDecoder: IWICBitmapDecoder): HRESULT; stdcall;

    function CreateEncoder(const guidContainerFormat: TGuid; const pguidVendor: PGUID;
      out ppIEncoder: IWICBitmapEncoder): HRESULT; stdcall;

    function CreatePalette(out ppIPalette: IWICPalette): HRESULT; stdcall;
    function CreateFormatConverter(out ppIFormatConverter: IWICFormatConverter): HRESULT; stdcall;
    function CreateBitmapScaler(out ppIBitmapScaler: IWICBitmapScaler): HRESULT; stdcall;
    function CreateBitmapClipper(out ppIBitmapClipper: IWICBitmapClipper): HRESULT; stdcall;
    function CreateBitmapFlipRotator(out ppIBitmapFlipRotator: IWICBitmapFlipRotator): HRESULT; stdcall;
    function CreateStream(out ppIWICStream: IWICStream): HRESULT; stdcall;
    function CreateColorContext(out ppIWICColorContext: IWICColorContext): HRESULT; stdcall;
    function CreateColorTransformer(out ppIWICColorTransform: IWICColorTransform): HRESULT; stdcall;
    function CreateBitmap(uiWidth: UINT; uiHeight: UINT; pixelFormat: REFWICPixelFormatGUID; option: WICBitmapCreateCacheOption;
                           out ppIBitmap: IWICBitmap): HRESULT; stdcall;

    function CreateBitmapFromSource(pIBitmapSource: IWICBitmapSource; option: WICBitmapCreateCacheOption;  
                                    out ppIBitmap: IWICBitmap): HRESULT; stdcall;

    function CreateBitmapFromSourceRect(pIBitmapSource: IWICBitmapSource; x: UINT; y: UINT; width: UINT; height: UINT;
                                         out ppIBitmap: IWICBitmap): HRESULT; stdcall;

    function CreateBitmapFromMemory(uiWidth: UINT; uiHeight: UINT;
      const pixelFormat: WICPixelFormatGUID; cbStride: UINT; cbBufferSize: UINT;
      pbBuffer: PByte; out ppIBitmap: IWICBitmap): HRESULT; stdcall;

    function CreateBitmapFromHBITMAP(hBitmap: HBITMAP; hPalette: HPALETTE; options: WICBitmapAlphaChannelOption;
                                      out ppIBitmap: IWICBitmap): HRESULT; stdcall;

    function CreateBitmapFromHICON(hIcon: HICON; out ppIBitmap: IWICBitmap): HRESULT; stdcall;
    function CreateComponentEnumerator(componentTypes: DWORD; options: DWORD; out ppIEnumUnknown: IEnumUnknown): HRESULT; stdcall;
    function CreateFastMetadataEncoderFromDecoder(pIDecoder: IWICBitmapDecoder;out ppIFastEncoder: IWICFastMetadataEncoder): HRESULT; stdcall;
    function CreateFastMetadataEncoderFromFrameDecode(pIFrameDecoder: IWICBitmapFrameDecode; 
                                                      out ppIFastEncoder: IWICFastMetadataEncoder): HRESULT; stdcall;

    function CreateQueryWriter(const guidMetadataFormat: TGuid;const pguidVendor: TGUID; 
                                out ppIQueryWriter: IWICMetadataQueryWriter): HRESULT; stdcall;

    function CreateQueryWriterFromReader(pIQueryReader: IWICMetadataQueryReader;const pguidVendor: TGUID;
                                          out ppIQueryWriter: IWICMetadataQueryWriter): HRESULT; stdcall;
  end;

const
  FACILITY_WINCODEC_ERR = $898;
  WINCODEC_ERR_BASE = $2000;

  WINCODEC_ERR_GENERIC_ERROR                    = E_FAIL;
  WINCODEC_ERR_INVALIDPARAMETER                 = E_INVALIDARG;
  WINCODEC_ERR_OUTOFMEMORY                      = E_OUTOFMEMORY;
  WINCODEC_ERR_NOTIMPLEMENTED                   = E_NOTIMPL;
  WINCODEC_ERR_ABORTED                          = E_ABORT;
  WINCODEC_ERR_ACCESSDENIED                     = E_ACCESSDENIED;
  WINCODEC_ERR_VALUEOVERFLOW                    = $80070216; //INTSAFE_E_ARITHMETIC_OVERFLOW;
  WINCODEC_ERR_WRONGSTATE                       = $88982f04;
  WINCODEC_ERR_VALUEOUTOFRANGE                  = $88982f05;
  WINCODEC_ERR_UNKNOWNIMAGEFORMAT               = $88982f07;
  WINCODEC_ERR_UNSUPPORTEDVERSION               = $88982f0B;
  WINCODEC_ERR_NOTINITIALIZED                   = $88982f0C;
  WINCODEC_ERR_ALREADYLOCKED                    = $88982f0D;
  WINCODEC_ERR_PROPERTYNOTFOUND                 = $88982f40;
  WINCODEC_ERR_PROPERTYNOTSUPPORTED             = $88982f41;
  WINCODEC_ERR_PROPERTYSIZE                     = $88982f42;
  WINCODEC_ERR_CODECPRESENT                     = $88982f43;
  WINCODEC_ERR_CODECNOTHUMBNAIL                 = $88982f44;
  WINCODEC_ERR_PALETTEUNAVAILABLE               = $88982f45;
  WINCODEC_ERR_CODECTOOMANYSCANLINES            = $88982f46;
  WINCODEC_ERR_INTERNALERROR                    = $88982f48;
  WINCODEC_ERR_SOURCERECTDOESNOTMATCHDIMENSIONS = $88982f49;
  WINCODEC_ERR_COMPONENTNOTFOUND                = $88982f50;
  WINCODEC_ERR_IMAGESIZEOUTOFRANGE              = $88982f51;
  WINCODEC_ERR_TOOMUCHMETADATA                  = $88982f52;
  WINCODEC_ERR_BADIMAGE                         = $88982f60;
  WINCODEC_ERR_BADHEADER                        = $88982f61;
  WINCODEC_ERR_FRAMEMISSING                     = $88982f62;
  WINCODEC_ERR_BADMETADATAHEADER                = $88982f63;
  WINCODEC_ERR_BADSTREAMDATA                    = $88982f70;
  WINCODEC_ERR_STREAMWRITE                      = $88982f71;
  WINCODEC_ERR_STREAMREAD                       = $88982f72;
  WINCODEC_ERR_STREAMNOTAVAILABLE               = $88982f73;
  WINCODEC_ERR_UNSUPPORTEDPIXELFORMAT           = $88982f80;
  WINCODEC_ERR_UNSUPPORTEDOPERATION             = $88982f81;
  WINCODEC_ERR_INVALIDREGISTRATION              = $88982f8A;
  WINCODEC_ERR_COMPONENTINITIALIZEFAILURE       = $88982f8B;
  WINCODEC_ERR_INSUFFICIENTBUFFER               = $88982f8C;
  WINCODEC_ERR_DUPLICATEMETADATAPRESENT         = $88982f8D;
  WINCODEC_ERR_PROPERTYUNEXPECTEDTYPE           = $88982f8E;
  WINCODEC_ERR_UNEXPECTEDSIZE                   = $88982f8F;
  WINCODEC_ERR_INVALIDQUERYREQUEST              = $88982f90;
  WINCODEC_ERR_UNEXPECTEDMETADATATYPE           = $88982f91;
  WINCODEC_ERR_REQUESTONLYVALIDATMETADATAROOT   = $88982f92;
  WINCODEC_ERR_INVALIDQUERYCHARACTER            = $88982f93;
  WINCODEC_ERR_WIN32ERROR                       = $88982f94;
  WINCODEC_ERR_INVALIDPROGRESSIVELEVEL          = $88982f95;

type
  WICTiffCompressionOption = type Integer;
const
  WICTiffCompressionDontCare           = 0;
  WICTiffCompressionNone               = $1;
  WICTiffCompressionCCITT3             = $2;
  WICTiffCompressionCCITT4             = $3;
  WICTiffCompressionLZW                = $4;
  WICTiffCompressionRLE                = $5;
  WICTiffCompressionZIP                = $6;
  WICTiffCompressionLZWHDifferencing   = $7;
  WICTIFFCOMPRESSIONOPTION_FORCE_DWORD = $7FFFFFFF;

type
  WICJpegYCrCbSubsamplingOption = type Integer;
const
  WICJpegYCrCbSubsamplingDefault      = 0;
  WICJpegYCrCbSubsampling420          = $1;
  WICJpegYCrCbSubsampling422          = $2;
  WICJpegYCrCbSubsampling444          = $3;
  WICJPEGYCRCBSUBSAMPLING_FORCE_DWORD = $7FFFFFFF;

type
  WICPngFilterOption = type Integer;
const
  WICPngFilterUnspecified        = 0;
  WICPngFilterNone               = $1;
  WICPngFilterSub                = $2;
  WICPngFilterUp                 = $3;
  WICPngFilterAverage            = $4;
  WICPngFilterPaeth              = $5;
  WICPngFilterAdaptive           = $6;
  WICPNGFILTEROPTION_FORCE_DWORD = $7FFFFFFF;

type
  WICNamedWhitePoint = type Integer;
const
  WICWhitePointDefault           = $1;
  WICWhitePointDaylight          = $2;
  WICWhitePointCloudy            = $4;
  WICWhitePointShade             = $8;
  WICWhitePointTungsten          = $10;
  WICWhitePointFluorescent       = $20;
  WICWhitePointFlash             = $40;
  WICWhitePointUnderwater        = $80;
  WICWhitePointCustom            = $100;
  WICWhitePointAutoWhiteBalance  = $200;
  WICWhitePointAsShot            = WICWHITEPOINTDEFAULT;
  WICNAMEDWHITEPOINT_FORCE_DWORD = $7FFFFFFF;

type
  WICRawCapabilities = type Integer;
const
  WICRawCapabilityNotSupported   = 0;
  WICRawCapabilityGetSupported   = $1;
  WICRawCapabilityFullySupported = $2;
  WICRAWCAPABILITIES_FORCE_DWORD = $7FFFFFFF;

type
  WICRawRotationCapabilities = type Integer;
const
  WICRawRotationCapabilityNotSupported           = 0;
  WICRawRotationCapabilityGetSupported           = $1;
  WICRawRotationCapabilityNinetyDegreesSupported = $2;
  WICRawRotationCapabilityFullySupported         = $3;
  WICRAWROTATIONCAPABILITIES_FORCE_DWORD         = $7FFFFFFF;

type
  WICRawCapabilitiesInfo = record
    cbSize: UINT;
    CodecMajorVersion: UINT;
    CodecMinorVersion: UINT;
    ExposureCompensationSupport: WICRawCapabilities;
    ContrastSupport: WICRawCapabilities;
    RGBWhitePointSupport: WICRawCapabilities;
    NamedWhitePointSupport: WICRawCapabilities;
    NamedWhitePointSupportMask: UINT;
    KelvinWhitePointSupport: WICRawCapabilities;
    GammaSupport: WICRawCapabilities;
    TintSupport: WICRawCapabilities;
    SaturationSupport: WICRawCapabilities;
    SharpnessSupport: WICRawCapabilities;
    NoiseReductionSupport: WICRawCapabilities;
    DestinationColorProfileSupport: WICRawCapabilities;
    ToneCurveSupport: WICRawCapabilities;
    RotationSupport: WICRawRotationCapabilities;
    RenderModeSupport: WICRawCapabilities;
  end;

type
  WICRawParameterSet = type Integer;
const
  WICAsShotParameterSet          = $1;
  WICUserAdjustedParameterSet    = $2;
  WICAutoAdjustedParameterSet    = $3;
  WICRAWPARAMETERSET_FORCE_DWORD = $7FFFFFFF;

type
  WICRawRenderMode = type Integer;
const
  WICRawRenderModeDraft        = $1;
  WICRawRenderModeNormal       = $2;
  WICRawRenderModeBestQuality  = $3;
  WICRAWRENDERMODE_FORCE_DWORD = $7FFFFFFF;

type
  WICRawToneCurvePoint = record
    Input: Double;
    Output: Double;
  end;

  WICRawToneCurve = record
    cPoints: UINT;
    apoints: array[0..0] of WICRawToneCurvePoint;
  end;
  TWICRawToneCurve = WICRawToneCurve;
  PWICRawToneCurve = ^TWICRawToneCurve;

const
  WICRawChangeNotification_ExposureCompensation    = $00000001;
  WICRawChangeNotification_NamedWhitePoint         = $00000002;
  WICRawChangeNotification_KelvinWhitePoint        = $00000004;
  WICRawChangeNotification_RGBWhitePoint           = $00000008;
  WICRawChangeNotification_Contrast                = $00000010;
  WICRawChangeNotification_Gamma                   = $00000020;
  WICRawChangeNotification_Sharpness               = $00000040;
  WICRawChangeNotification_Saturation              = $00000080;
  WICRawChangeNotification_Tint                    = $00000100;
  WICRawChangeNotification_NoiseReduction          = $00000200;
  WICRawChangeNotification_DestinationColorContext = $00000400;
  WICRawChangeNotification_ToneCurve               = $00000800;
  WICRawChangeNotification_Rotation                = $00001000;
  WICRawChangeNotification_RenderMode              = $00002000;

{ interface IWICDevelopRawNotificationCallback }
type
  IWICDevelopRawNotificationCallback = interface(IUnknown)
    [SID_IWICDevelopRawNotificationCallback]
    function Notify(NotificationMask: UINT): HRESULT; stdcall;
  end;

{ interface IWICDevelopRaw }
  IWICDevelopRaw = interface(IWICBitmapFrameDecode)
    [SID_IWICDevelopRaw]
    function QueryRawCapabilitiesInfo(var pInfo: WICRawCapabilitiesInfo): HRESULT; stdcall;
    function LoadParameterSet(ParameterSet: WICRawParameterSet): HRESULT; stdcall;
    function GetCurrentParameterSet(out ppCurrentParameterSet: IUnknown): HRESULT; stdcall;
    function SetExposureCompensation(ev: Double): HRESULT; stdcall;
    function GetExposureCompensation(var pEV: Double): HRESULT; stdcall;
    function SetWhitePointRGB(Red: UINT; Green: UINT; Blue: UINT): HRESULT; stdcall;
    function GetWhitePointRGB(var pRed: UINT; var pGreen: UINT; var pBlue: UINT): HRESULT; stdcall;
    function SetNamedWhitePoint(WhitePoint: WICNamedWhitePoint): HRESULT; stdcall;
    function GetNamedWhitePoint(var pWhitePoint: WICNamedWhitePoint): HRESULT; stdcall;
    function SetWhitePointKelvin(WhitePointKelvin: UINT): HRESULT; stdcall;
    function GetWhitePointKelvin(var pWhitePointKelvin: UINT): HRESULT; stdcall;
    function GetKelvinRangeInfo(var pMinKelvinTemp: UINT; var pMaxKelvinTemp: UINT; var pKelvinTempStepValue: UINT): HRESULT; stdcall;
    function SetContrast(Contrast: Double): HRESULT; stdcall;
    function GetContrast(var pContrast: Double): HRESULT; stdcall;
    function SetGamma(Gamma: Double): HRESULT; stdcall;
    function GetGamma(var pGamma: Double): HRESULT; stdcall;
    function SetSharpness(Sharpness: Double): HRESULT; stdcall;
    function GetSharpness(var pSharpness: Double): HRESULT; stdcall;
    function SetSaturation(Saturation: Double): HRESULT; stdcall;
    function GetSaturation(var pSaturation: Double): HRESULT; stdcall;
    function SetTint(Tint: Double): HRESULT; stdcall;
    function GetTint(var pTint: Double): HRESULT; stdcall;
    function SetNoiseReduction(NoiseReduction: Double): HRESULT; stdcall;
    function GetNoiseReduction(var pNoiseReduction: Double): HRESULT; stdcall;
    function SetDestinationColorContext(pColorContext: IWICColorContext): HRESULT; stdcall;
    function SetToneCurve(cbToneCurveSize: UINT; pToneCurve: PWICRawToneCurve): HRESULT; stdcall;
    function GetToneCurve(cbToneCurveBufferSize: UINT; pToneCurve: PWICRawToneCurve; var pcbActualToneCurveBufferSize: UINT): HRESULT; stdcall;
    function SetRotation(Rotation: Double): HRESULT; stdcall;
    function GetRotation(var pRotation: Double): HRESULT; stdcall;
    function SetRenderMode(RenderMode: WICRawRenderMode): HRESULT; stdcall;
    function GetRenderMode(var pRenderMode: WICRawRenderMode): HRESULT; stdcall;
    function SetNotificationCallback(pCallback: IWICDevelopRawNotificationCallback): HRESULT; stdcall;
  end;


implementation

end.
