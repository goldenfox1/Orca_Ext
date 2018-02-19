{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

var
  DrakonBtnPath:array[TD2DrakonIcon] of string=(
    //drdSilhouette =
    'M 309.929,269.092 L 309.929,266.943 L 313.746,268.877 L 313.746,265.815 L 309.929,265.815 C 307.643,265.567 306.498,264.332 306.498,262.109 L 306.498,262.109 C 306.498,259.679 307.770,258.460 310.315,258.457 L 310.315,258.457 L 317.952,258.457 C 320.460,258.460 321.712,259.679 321.712,262.109 L 321.712,262.109 C 321.712,264.332 320.587,265.567 318.338,265.815 L 318.338,265.815 L 314.521,265.815 L 314.521,269.092 L 328.959,269.092 L 328.959,275.000 L 330.732,275.000 C 332.283,275.003 333.055,275.755 333.055,277.256 L 333.055,277.256 C 333.055,278.548 332.410,279.283 331.118,279.458 L 331.118,279.458 L 331.118,279.512 L 326.029,279.512 L 326.029,279.458 C 324.740,279.283 324.092,278.548 324.092,277.256 L 324.092,277.256 C 324.092,275.755 324.867,275.003 326.415,275.000 L 326.415,275.000 L 328.184,275.000 L 328.184,269.844 L 321.712,269.844 L 321.712,284.453 L 306.498,284.453 L 306.498,269.092 Z M 330.342,278.760 L 330.732,278.760 C 331.766,278.763 332.279,278.263 332.279,277.256 L 332.279,277.256 C 332.279,276.256 331.766,275.755 330.732,275.752 L 330.732,275.752 L 326.415,275.752 C 325.384,275.755 324.867,276.256 324.867,277.256 L 324.867,277.256 C 324.867,278.263 325.384,278.763 326.415,278.760 L 326.415,278.760 L 326.801,278.760 Z M 317.563,265.064 L 317.952,265.064 C 319.980,265.067 320.993,264.083 320.993,262.109 L 320.993,262.109 C 320.993,260.142 319.980,259.159 317.952,259.155 L 317.952,259.155 L 310.315,259.155 C 308.287,259.159 307.274,260.142 307.274,262.109 L 307.274,262.109 C 307.274,264.083 308.287,265.067 310.315,265.064 L 310.315,265.064 L 310.647,265.064 Z M 309.929,269.844 L 307.274,269.844 L 307.274,283.755 L 313.746,283.755 L 313.746,270.059 L 309.929,271.992 Z M 314.521,269.844 L 314.521,283.755 L 320.993,283.755 L 320.993,269.844 Z',
    //drdBranch
    'M 320.111,276.558 L 320.111,284.453 L 319.335,284.453 L 319.335,276.558 L 307.774,270.166 L 307.774,263.291 L 319.335,263.291 L 319.335,259.155 L 306.334,259.155 L 306.334,258.457 L 333.112,258.457 L 333.112,259.155 L 320.111,259.155 L 320.111,263.291 L 331.672,263.291 L 331.672,270.166 Z M 319.724,275.913 L 330.896,269.736 L 330.896,264.043 L 308.549,264.043 L 308.549,269.736 Z',
    //drdAction
    'M 320.325,276.235 L 320.325,284.453 L 319.550,284.453 L 319.550,276.235 L 306.992,276.235 L 306.992,266.675 L 319.550,266.675 L 319.550,258.457 L 320.325,258.457 L 320.325,266.675 L 332.884,266.675 L 332.884,276.235 Z M 307.767,267.373 L 307.767,275.537 L 332.108,275.537 L 332.108,267.373 Z',
    //drdQuestion =
    'M 318.449,280.640 L 332.279,280.640 L 332.279,270.327 L 329.624,270.327 L 326.361,275.913 L 318.449,275.913 Z M 310.208,264.741 L 307.220,269.951 L 310.208,275.161 L 325.918,275.161 L 328.963,269.951 L 325.918,264.741 Z M 318.449,281.392 L 318.449,284.453 L 317.730,284.453 L 317.730,275.913 L 309.764,275.913 L 306.334,269.951 L 309.764,263.989 L 317.788,263.989 L 317.788,258.457 L 318.506,258.457 L 318.506,263.989 L 326.361,263.989 L 329.624,269.575 L 333.055,269.575 L 333.055,281.392 Z',
    //drdSelect =
    'M 320.809,282.788 L 320.809,284.453 L 320.033,284.453 L 320.033,282.788 L 308.472,276.397 L 308.472,269.522 L 320.033,269.522 L 320.033,268.555 L 308.361,268.555 L 310.849,260.337 L 320.033,260.337 L 320.033,258.457 L 320.809,258.457 L 320.809,260.337 L 332.481,260.337 L 329.993,268.555 L 320.809,268.555 L 320.809,269.522 L 332.370,269.522 L 332.370,276.397 Z M 311.403,261.089 L 309.358,267.856 L 329.439,267.856 L 331.484,261.089 Z M 320.423,282.144 L 330.601,276.504 L 310.241,276.504 Z M 331.595,275.806 L 331.595,270.273 L 309.247,270.273 L 309.247,275.806 Z',
    //drdCase =
    'M 307.774,270.166 L 307.774,263.291 L 319.335,263.291 L 319.335,259.155 L 306.334,259.155 L 306.334,258.457 L 333.112,258.457 L 333.112,259.155 L 320.111,259.155 L 320.111,263.291 L 331.672,263.291 L 331.672,270.166 L 320.111,276.558 L 320.111,284.453 L 319.335,284.453 L 319.335,276.558 Z M 330.896,269.575 L 330.896,264.043 L 308.549,264.043 L 308.549,269.575 Z M 309.543,270.273 L 319.724,275.913 L 329.903,270.273 Z',
    //drdInsertion =
    'M 320.325,276.235 L 320.325,284.453 L 319.550,284.453 L 319.550,276.235 L 311.916,276.235 L 311.141,276.235 L 306.992,276.235 L 306.992,266.675 L 311.141,266.675 L 311.916,266.675 L 319.550,266.675 L 319.550,258.457 L 320.325,258.457 L 320.325,266.675 L 327.959,266.675 L 328.734,266.675 L 332.884,266.675 L 332.884,276.235 L 328.734,276.235 L 327.959,276.235 Z M 328.734,267.373 L 328.734,275.537 L 332.108,275.537 L 332.108,267.373 Z M 327.959,275.537 L 327.959,267.373 L 311.916,267.373 L 311.916,275.537 Z M 311.141,275.537 L 311.141,267.373 L 307.767,267.373 L 307.767,275.537 Z',
    //drdParams =
    'M 314.283,271.831 L 308.086,271.831 L 308.086,271.079 L 314.283,271.079 L 314.283,267.105 L 332.541,267.105 L 332.541,275.806 L 314.283,275.806 Z M 315.058,267.803 L 315.058,275.107 L 331.819,275.107 L 331.819,267.803 Z',
    //drdLoop =
    'M 320.950,273.442 L 320.950,269.468 L 310.772,269.468 L 310.772,265.064 L 314.699,261.196 L 320.728,261.196 L 320.728,258.457 L 321.504,258.457 L 321.504,261.196 L 327.479,261.196 L 331.460,265.064 L 331.460,269.468 L 321.725,269.468 L 321.725,273.442 L 331.682,273.442 L 331.682,277.847 L 327.754,281.714 L 321.725,281.714 L 321.725,284.453 L 320.950,284.453 L 320.950,281.714 L 314.974,281.714 L 310.993,277.847 L 310.993,273.442 Z M 311.765,274.194 L 311.765,277.524 L 315.253,280.962 L 327.422,280.962 L 330.963,277.524 L 330.963,274.194 Z M 311.490,268.716 L 330.688,268.716 L 330.688,265.386 L 327.200,261.948 L 315.031,261.948 L 311.490,265.386 Z',
    //drdComment =
    'M 320.111,281.821 L 320.111,284.453 L 319.335,284.453 L 319.335,281.821 L 306.334,281.821 L 306.334,261.089 L 319.335,261.089 L 319.335,258.457 L 320.111,258.457 L 320.111,261.089 L 333.112,261.089 L 333.112,281.821 Z M 311.812,263.237 L 327.633,263.237 C 329.406,263.241 330.289,264.117 330.289,265.869 L 330.289,265.869 L 330.289,277.041 C 330.289,278.800 329.406,279.676 327.633,279.673 L 327.633,279.673 L 311.812,279.673 C 310.043,279.676 309.157,278.800 309.157,277.041 L 309.157,277.041 L 309.157,265.869 C 309.157,264.117 310.043,263.241 311.812,263.237 L 311.812,263.237 Z M 311.812,263.989 C 310.523,263.993 309.875,264.620 309.875,265.869 L 309.875,265.869 L 309.875,277.041 C 309.875,278.297 310.523,278.924 311.812,278.921 L 311.812,278.921 L 327.633,278.921 C 328.926,278.924 329.570,278.297 329.570,277.041 L 329.570,277.041 L 329.570,265.869 C 329.570,264.620 328.926,263.993 327.633,263.989 L 327.633,263.989 Z M 307.055,261.841 L 307.055,281.069 L 332.390,281.069 L 332.390,261.841 Z',
    //drdCommentL =
    'M 329.184,271.079 L 333.055,271.079 L 333.055,271.831 L 329.184,271.831 C 327.304,271.834 326.361,272.784 326.361,274.678 L 326.361,274.678 L 326.361,280.855 C 326.361,282.362 325.717,283.436 324.424,284.077 L 324.424,284.077 C 323.837,284.332 323.303,284.457 322.820,284.453 L 322.820,284.453 L 322.487,284.453 L 322.487,283.701 L 322.820,283.701 C 323.229,283.705 323.652,283.614 324.092,283.433 L 324.092,283.433 C 325.126,282.899 325.643,282.040 325.643,280.855 L 325.643,280.855 L 325.643,274.678 C 325.643,273.177 326.291,272.103 327.580,271.455 L 327.580,271.455 C 326.291,270.851 325.643,269.777 325.643,268.232 L 325.643,268.232 L 325.643,262.056 C 325.643,260.914 325.126,260.055 324.092,259.478 L 324.092,259.478 C 323.652,259.303 323.229,259.212 322.820,259.209 L 322.820,259.209 L 322.487,259.209 L 322.487,258.457 L 322.820,258.457 C 323.303,258.460 323.837,258.585 324.424,258.833 L 324.424,258.833 C 325.717,259.518 326.361,260.592 326.361,262.056 L 326.361,262.056 L 326.361,268.232 C 326.361,270.132 327.304,271.082 329.184,271.079 L 329.184,271.079 Z M 319.832,263.667 L 321.769,266.567 L 321.104,266.997 L 319.446,264.365 L 317.730,266.997 L 317.066,266.567 L 318.949,263.667 L 317.066,260.767 L 317.730,260.337 L 319.446,262.969 L 321.104,260.337 L 321.769,260.767 Z M 314.467,263.667 L 316.404,266.567 L 315.740,266.997 L 314.024,264.365 L 312.309,266.997 L 311.644,266.567 L 313.581,263.667 L 311.644,260.767 L 312.309,260.337 L 314.024,262.969 L 315.740,260.337 L 316.404,260.767 Z M 309.046,263.667 L 310.983,266.567 L 310.318,266.997 L 308.603,264.365 L 306.888,266.997 L 306.280,266.567 L 308.160,263.667 L 306.280,260.767 L 306.888,260.337 L 308.603,262.969 L 310.318,260.337 L 310.983,260.767 Z M 319.832,271.455 L 321.769,274.355 L 321.104,274.785 L 319.446,272.153 L 317.730,274.785 L 317.066,274.355 L 318.949,271.455 L 317.066,268.555 L 317.730,268.125 L 319.446,270.757 L 321.104,268.125 L 321.769,268.555 Z M 314.467,271.455 L 316.404,274.355 L 315.740,274.785 L 314.024,272.153 L 312.309,274.785 L 311.644,274.355 L 313.581,271.455 L 311.644,268.555 L 312.309,268.125 L 314.024,270.757 L 315.740,268.125 L 316.404,268.555 Z M 309.046,271.455 L 310.983,274.355 L 310.318,274.785 L 308.603,272.153 L 306.888,274.785 L 306.280,274.355 L 308.160,271.455 L 306.280,268.555 L 306.888,268.125 L 308.603,270.757 L 310.318,268.125 L 310.983,268.555 Z M 319.832,279.243 L 321.769,282.144 L 321.104,282.573 L 319.446,279.941 L 317.730,282.573 L 317.066,282.144 L 318.949,279.243 L 317.066,276.343 L 317.730,275.913 L 319.446,278.545 L 321.104,275.913 L 321.769,276.343 Z M 314.467,279.243 L 316.404,282.144 L 315.740,282.573 L 314.024,279.941 L 312.309,282.573 L 311.644,282.144 L 313.581,279.243 L 311.644,276.343 L 312.309,275.913 L 314.024,278.545 L 315.740,275.913 L 316.404,276.343 Z M 309.046,279.243 L 310.983,282.144 L 310.318,282.573 L 308.603,279.941 L 306.888,282.573 L 306.280,282.144 L 308.160,279.243 L 306.280,276.343 L 306.888,275.913 L 308.603,278.545 L 310.318,275.913 L 310.983,276.343 Z',
    //drdCommentR =
    'M 310.208,271.831 L 306.337,271.831 L 306.337,271.079 L 310.208,271.079 C 312.091,271.082 313.031,270.132 313.031,268.232 L 313.031,268.232 L 313.031,262.056 C 313.031,260.592 313.679,259.518 314.968,258.833 L 314.968,258.833 C 315.522,258.585 316.055,258.460 316.572,258.457 L 316.572,258.457 L 316.905,258.457 L 316.905,259.209 L 316.572,259.209 C 316.129,259.212 315.706,259.303 315.300,259.478 L 315.300,259.478 C 314.266,260.055 313.749,260.914 313.749,262.056 L 313.749,262.056 L 313.749,268.232 C 313.749,269.777 313.105,270.851 311.812,271.455 L 311.812,271.455 C 313.105,272.103 313.749,273.177 313.749,274.678 L 313.749,274.678 L 313.749,280.855 C 313.749,282.040 314.266,282.899 315.300,283.433 L 315.300,283.433 C 315.706,283.614 316.129,283.705 316.572,283.701 L 316.572,283.701 L 316.905,283.701 L 316.905,284.453 L 316.572,284.453 C 316.055,284.457 315.522,284.332 314.968,284.077 L 314.968,284.077 C 313.679,283.436 313.031,282.362 313.031,280.855 L 313.031,280.855 L 313.031,274.678 C 313.031,272.784 312.091,271.834 310.208,271.831 L 310.208,271.831 Z M 319.946,262.969 L 321.661,260.337 L 322.326,260.767 L 320.443,263.667 L 322.326,266.567 L 321.661,266.997 L 319.946,264.365 L 318.288,266.997 L 317.623,266.567 L 319.560,263.667 L 317.623,260.767 L 318.288,260.337 Z M 325.367,262.969 L 327.083,260.337 L 327.748,260.767 L 325.811,263.667 L 327.748,266.567 L 327.083,266.997 L 325.367,264.365 L 323.652,266.997 L 322.987,266.567 L 324.924,263.667 L 322.987,260.767 L 323.652,260.337 Z M 330.789,262.969 L 332.504,260.337 L 333.112,260.767 L 331.232,263.667 L 333.112,266.567 L 332.504,266.997 L 330.789,264.365 L 329.074,266.997 L 328.409,266.567 L 330.346,263.667 L 328.409,260.767 L 329.074,260.337 Z M 319.946,270.757 L 321.661,268.125 L 322.326,268.555 L 320.443,271.455 L 322.326,274.355 L 321.661,274.785 L 319.946,272.153 L 318.288,274.785 L 317.623,274.355 L 319.560,271.455 L 317.623,268.555 L 318.288,268.125 Z M 325.367,270.757 L 327.083,268.125 L 327.748,268.555 L 325.811,271.455 L 327.748,274.355 L 327.083,274.785 L 325.367,272.153 L 323.652,274.785 L 322.987,274.355 L 324.924,271.455 L 322.987,268.555 L 323.652,268.125 Z M 330.789,270.757 L 332.504,268.125 L 333.112,268.555 L 331.232,271.455 L 333.112,274.355 L 332.504,274.785 L 330.789,272.153 L 329.074,274.785 L 328.409,274.355 L 330.346,271.455 L 328.409,268.555 L 329.074,268.125 Z M 319.946,278.545 L 321.661,275.913 L 322.326,276.343 L 320.443,279.243 L 322.326,282.144 L 321.661,282.573 L 319.946,279.941 L 318.288,282.573 L 317.623,282.144 L 319.560,279.243 L 317.623,276.343 L 318.288,275.913 Z M 325.367,278.545 L 327.083,275.913 L 327.748,276.343 L 325.811,279.243 L 327.748,282.144 L 327.083,282.573 L 325.367,279.941 L 323.652,282.573 L 322.987,282.144 L 324.924,279.243 L 322.987,276.343 L 323.652,275.913 Z M 330.789,278.545 L 332.504,275.913 L 333.112,276.343 L 331.232,279.243 L 333.112,282.144 L 332.504,282.573 L 330.789,279.941 L 329.074,282.573 L 328.409,282.144 L 330.346,279.243 L 328.409,276.343 L 329.074,275.913 Z',
    //drdParallel =
    'M 310.741,281.821 L 308.415,275.430 L 310.741,275.430 L 310.741,258.457 L 311.513,258.457 L 311.513,260.659 L 330.101,260.659 L 330.101,275.430 L 332.427,275.430 L 329.993,282.144 L 311.513,282.144 L 311.513,284.453 L 310.741,284.453 Z M 311.681,281.392 L 319.869,281.392 L 317.710,275.430 L 320.033,275.430 L 320.033,263.613 L 311.513,263.613 L 311.513,275.430 L 313.836,275.430 Z M 311.513,262.861 L 320.033,262.861 L 320.033,261.411 L 311.513,261.411 Z M 320.920,281.392 L 329.161,281.392 L 327.006,275.430 L 329.329,275.430 L 329.329,263.613 L 320.752,263.613 L 320.752,275.430 L 323.078,275.430 Z M 320.752,262.861 L 329.329,262.861 L 329.329,261.411 L 320.752,261.411 Z',
    //drdMoreParallel =
    'M 327.358,275.483 L 329.681,275.483 L 327.247,282.197 L 317.287,282.197 L 317.287,281.445 L 326.472,281.445 L 324.313,275.483 L 326.636,275.483 L 326.636,263.667 L 317.344,263.667 L 317.344,262.915 L 326.636,262.915 L 326.636,261.465 L 317.287,261.465 L 317.287,260.713 L 327.358,260.713 Z',
    //drdInput =
    'M 319.896,280.693 L 319.896,284.453 L 319.120,284.453 L 319.120,280.693 L 306.397,280.693 L 306.397,270.273 L 311.319,270.273 L 311.319,262.217 L 319.120,262.217 L 319.120,258.457 L 319.896,258.457 L 319.896,262.217 L 333.229,262.217 L 327.086,267.427 L 333.229,272.637 L 327.697,272.637 L 327.697,280.693 Z M 307.170,271.025 L 307.170,279.941 L 326.922,279.941 L 326.922,271.025 Z M 319.896,262.969 L 312.094,262.969 L 312.094,270.273 L 327.697,270.273 L 327.697,271.885 L 331.235,271.885 L 325.982,267.427 L 331.235,262.969 Z',
    //drdOutput =
    'M 319.560,280.693 L 319.560,284.453 L 318.785,284.453 L 318.785,280.693 L 306.226,280.693 L 306.226,270.273 L 310.986,270.273 L 310.986,262.217 L 318.785,262.217 L 318.785,258.457 L 319.560,258.457 L 319.560,262.217 L 328.855,262.217 L 333.115,267.427 L 328.855,272.637 L 327.361,272.637 L 327.361,280.693 Z M 306.948,271.025 L 306.948,279.941 L 326.586,279.941 L 326.586,271.025 Z M 319.560,262.969 L 311.758,262.969 L 311.758,270.273 L 327.361,270.273 L 327.361,271.885 L 328.466,271.885 L 332.172,267.427 L 328.466,262.969 Z',
    //drdSInput =
    'M 321.185,276.665 L 321.185,284.775 L 320.409,284.775 L 320.409,276.665 L 310.120,276.665 L 310.120,266.245 L 320.409,266.245 L 320.409,258.135 L 321.185,258.135 L 321.185,266.245 L 332.081,266.245 L 325.941,271.455 L 332.081,276.665 Z M 310.896,275.913 L 330.037,275.913 L 324.780,271.455 L 330.037,266.997 L 310.896,266.997 Z',
    //drdSOutput =
    'M 319.137,276.665 L 319.137,284.453 L 318.415,284.453 L 318.415,276.665 L 309.788,276.665 L 309.788,266.245 L 318.415,266.245 L 318.415,258.457 L 319.137,258.457 L 319.137,266.245 L 327.600,266.245 L 332.081,271.455 L 327.600,276.665 Z M 310.506,275.913 L 327.267,275.913 L 331.084,271.455 L 327.267,266.997 L 310.506,266.997 Z',
    //drdProcess =
    'M 320.218,280.693 L 320.218,284.453 L 319.442,284.453 L 319.442,280.693 L 306.720,280.693 L 306.720,270.273 L 311.641,270.273 L 311.641,262.217 L 319.442,262.217 L 319.442,258.457 L 320.218,258.457 L 320.218,262.217 L 332.941,262.217 L 332.941,272.637 L 328.019,272.637 L 328.019,280.693 Z M 307.492,271.025 L 307.492,279.941 L 327.244,279.941 L 327.244,271.025 Z M 320.218,262.969 L 312.416,262.969 L 312.416,270.273 L 328.019,270.273 L 328.019,271.885 L 332.169,271.885 L 332.169,262.969 Z',
    //drdShelf =
    'M 307.055,270.273 L 332.390,270.273 L 332.390,262.969 L 307.055,262.969 Z M 333.112,280.693 L 320.111,280.693 L 320.111,284.453 L 319.335,284.453 L 319.335,280.693 L 306.334,280.693 L 306.334,262.217 L 319.335,262.217 L 319.335,258.457 L 320.111,258.457 L 320.111,262.217 L 333.112,262.217 Z M 332.390,271.025 L 307.055,271.025 L 307.055,279.941 L 332.390,279.941 Z',
    //drdTimer =
    'M 320.164,276.235 L 320.164,284.453 L 319.389,284.453 L 319.389,276.235 L 312.363,276.235 L 306.555,266.675 L 319.389,266.675 L 319.389,258.457 L 320.164,258.457 L 320.164,266.675 L 332.998,266.675 L 327.190,276.235 Z M 311.644,267.373 L 316.569,275.537 L 322.984,275.537 L 327.909,267.373 Z M 315.683,275.537 L 310.758,267.373 L 307.881,267.373 L 312.752,275.537 Z M 328.795,267.373 L 323.870,275.537 L 326.801,275.537 L 331.672,267.373 Z',
    //drdDuration =
    'M 332.199,271.079 L 332.199,271.831 L 327.993,271.831 L 325.837,275.430 L 313.611,275.430 L 308.798,267.480 L 330.594,267.480 L 328.436,271.079 Z M 310.124,268.179 L 314.051,274.731 L 325.394,274.731 L 329.322,268.179 Z',
    //drdPause =
    'M 320.164,276.235 L 320.164,284.453 L 319.389,284.453 L 319.389,276.235 L 312.363,276.235 L 306.555,266.675 L 319.389,266.675 L 319.389,258.457 L 320.164,258.457 L 320.164,266.675 L 332.998,266.675 L 327.190,276.235 Z M 307.881,267.373 L 312.752,275.537 L 326.801,275.537 L 331.672,267.373 Z',
    //drdZoom =
    'M 324.253,274.678 L 332.941,283.057 L 332.941,284.077 L 331.833,284.131 L 323.202,275.698 C 321.359,277.313 319.164,278.119 316.619,278.115 L 316.619,278.115 C 313.820,278.119 311.429,277.172 309.456,275.272 L 309.456,275.272 C 307.482,273.375 306.495,271.066 306.495,268.340 L 306.495,268.340 C 306.495,265.658 307.482,263.348 309.456,261.415 L 309.456,261.415 C 311.429,259.481 313.820,258.514 316.619,258.511 L 316.619,258.511 C 319.385,258.514 321.755,259.481 323.729,261.415 L 323.729,261.415 C 325.703,263.348 326.690,265.658 326.690,268.340 L 326.690,268.340 C 326.690,270.777 325.878,272.892 324.253,274.678 L 324.253,274.678 Z M 316.233,268.018 L 316.233,263.989 L 317.009,263.989 L 317.009,268.018 L 321.376,268.018 L 321.376,268.716 L 317.009,268.716 L 317.009,272.744 L 316.233,272.744 L 316.233,268.716 L 311.863,268.716 L 311.863,268.018 Z M 316.619,259.263 C 314.001,259.266 311.789,260.162 309.983,261.952 L 309.983,261.952 C 308.177,263.744 307.270,265.873 307.270,268.340 L 307.270,268.340 C 307.270,270.851 308.177,272.989 309.983,274.762 L 309.983,274.762 C 311.789,276.534 314.001,277.420 316.619,277.417 L 316.619,277.417 C 319.164,277.420 321.349,276.534 323.175,274.762 L 323.175,274.762 C 325.002,272.989 325.915,270.851 325.915,268.340 L 325.915,268.340 C 325.915,265.873 325.002,263.744 323.175,261.952 L 323.175,261.952 C 321.349,260.162 319.164,259.266 316.619,259.263 L 316.619,259.263 Z'

    ////drdHome =
    //'M 333.112,271.294 L 332.558,271.777 L 330.235,269.522 L 330.235,282.358 C 330.235,283.758 329.496,284.457 328.019,284.453 L 328.019,284.453 L 311.426,284.453 C 309.949,284.457 309.211,283.758 309.211,282.358 L 309.211,282.358 L 309.211,269.522 L 306.888,271.777 L 306.334,271.294 L 319.724,258.296 L 325.089,263.506 L 325.089,261.250 L 329.792,261.250 L 329.792,268.071 Z M 329.460,268.770 L 319.724,259.316 L 309.986,268.770 L 309.986,282.358 C 309.986,283.292 310.466,283.758 311.426,283.755 L 311.426,283.755 L 315.354,283.755 L 315.354,273.711 L 324.092,273.711 L 324.092,283.755 L 328.019,283.755 C 328.980,283.758 329.460,283.292 329.460,282.358 L 329.460,282.358 Z M 316.072,283.755 L 323.373,283.755 L 323.373,274.463 L 316.072,274.463 Z M 325.864,264.258 L 329.016,267.319 L 329.016,261.948 L 325.864,261.948 Z ',
    ////drdProperty =
    //'M 325.240,263.398 L 327.949,263.398 L 325.129,260.498 L 322.803,260.498 L 319.983,263.398 Z M 328.503,264.150 L 325.958,264.150 L 325.958,274.517 L 328.503,274.517 Z M 328.503,275.215 L 319.429,275.215 L 319.429,277.148 L 328.503,277.148 Z M 318.711,275.215 L 318.711,274.517 L 318.711,263.613 L 323.467,258.779 C 323.763,258.497 324.095,258.514 324.464,258.833 L 324.464,258.833 L 329.221,263.613 L 329.221,281.445 C 329.221,283.453 328.191,284.457 326.123,284.453 L 326.123,284.453 L 321.809,284.453 C 319.745,284.457 318.711,283.453 318.711,281.445 L 318.711,281.445 L 318.711,277.900 L 318.711,277.148 Z M 319.429,274.517 L 321.974,274.517 L 321.974,264.150 L 319.429,264.150 Z M 319.429,277.900 L 319.429,281.445 C 319.429,282.990 320.225,283.758 321.809,283.755 L 321.809,283.755 L 326.123,283.755 C 327.711,283.758 328.503,282.990 328.503,281.445 L 328.503,281.445 L 328.503,277.900 Z M 325.240,264.150 L 322.692,264.150 L 322.692,274.517 L 325.240,274.517 Z'
    );

  DrakonBtnHints:array[TD2DrakonIcon] of string=(
    //drhSilhouette =
    'Cилуэт/Примитив',
    //drhBranch =
    'Новая ветка',
    //drhAction =
    'Действие',
    //drhQuestion =
    'Вопрос',
    //drhSelect =
    'Множественный выбор из нескольких значений',
    //drhCase =
    'Вариант для множественного выбора',
    //drhInsertion =
    'Вставка',
    //drhParams =
    'Вхдные параметры',
     //drhLoop =
    'Цикл',
    //drhComment =
    'Комментарий',
    //drhCommentL =
    'Комментарий слева',
    //drhCommentR =
    'Комментарий справа',
    //drhParallel =
    'Одновременная работа',
    //drhMoreParallel  =
    'Добавить одновременное исполнение',
    //drhInput =
    'Ввод',
    //drhOutput =
    'Вывод',
     //drhSInput =
    'Простой ввод',
    //drhSOutput =
    'Простой вывод',
    //drhProcess =
    'Парралельный процесс',
    //drhShelf =
    'Полка',
    //drhTimer =
    'Пуск таймера',
    //drhDuration =
    'Синхронизатор (по таймеру)',
    //drhPause =
    'Пауза',
    //drhZoom =
    'Масштаб'
    ////drhHome =
    //'К началу диаграммы',
    ////drhProperty =
    //'Свойства иконы'
    );


//==================== TD2DrakonButton =======================================

procedure TD2DrakonButton.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('path');
  if (T <> nil) and (T is TD2Path) then
  begin
    TD2Path(T).WrapMode:=d2PathFit;
  end;
end;

constructor TD2DrakonButton.Create(AOwner: TComponent);
begin
  inherited;
  CanFocused:=false;
  Locked:=true;
  Stored:=false;
  ShowHint:=True;
  PathSize:=20;
  PathLayout:=d2GlyphCenter;
  PathStroke.Style:=d2BrushNone;
end;

//==================== TD2DrakonPallet =======================================

procedure TD2DrakonPallet.ClickHandler(Sender: TObject);
begin
  if FEditor <> nil
    then FEditor.SetCommand(TD2DrakonButton(Sender).Index);
end;

procedure TD2DrakonPallet.InitButtons;
var
  I:TD2DrakonIcon;
begin
  for I:=Low(Buttons) to High(Buttons) do
  begin
    Buttons[I]:=TD2DrakonButton.Create (Self);
    Buttons[I].Index:=I;
    Buttons[I].Parent:=Self;
    Buttons[I].Path.Data:=DrakonBtnPath[I];
    Buttons[I].Hint:=DrakonBtnHints[I];
    Buttons[I].FOnClick := ClickHandler;
    Buttons[I].Enabled:=False;
  end;
  RealignButtons;
end;

procedure TD2DrakonPallet.Realign;
begin
  if csLoading in ComponentState then exit;
  if csDestroying in ComponentState then Exit;
  if FDisableDefaultAlign then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;
  RealignButtons;
  inherited;
end;

procedure TD2DrakonPallet.RealignButtons;
var
  I:TD2DrakonIcon;
  X,Y:single;
  t: integer;
begin
  X:=0;
  Y:=0;
  t:=1;
  for I:=Low(Buttons) to High(Buttons) do
  begin
    if I in FIcons
      then  begin
              Buttons[I].Visible:=true;
              Buttons[I].SetBounds(X,Y,FMinBtnSize.X,FMinBtnSize.Y);
              if FOrientation=d2Vertical
                then  begin
                        if t=FLines
                          then  begin
                                  X:=0;
                                  Y:=Y + FMinBtnSize.Y + FInterval;
                                  t:=1;
                                end
                          else  begin
                                  X:=X + FMinBtnSize.X + FInterval;
                                  inc(t);
                                end;
                      end
                else  begin
                        if t=FLines
                          then  begin
                                  X:=X + FMinBtnSize.X + FInterval;
                                  Y:=0;
                                  t:=1;
                                end
                          else  begin
                                  Y:=Y + FMinBtnSize.Y + FInterval;
                                  inc(t);
                                end;
                      end;
            end
      else Buttons[I].Visible:=false;
  end;
  if FOrientation=d2Vertical
    then  begin
            Width:=FLines*FMinBtnSize.X+(Lines-1)*FInterval;
            if t > 1
              then Height:=Y+FMinBtnSize.Y
              else Height:=Y-FInterval;

          end
    else  begin
            Height:=FLines*FMinBtnSize.Y+(Lines-1)*FInterval;
            if t > 1
              then Width:=X+FMinBtnSize.X
              else Width:=X-FInterval;
          end
end;

procedure TD2DrakonPallet.SetEditor(AValue: TD2CustomDrakonEditor);
var I:TD2DrakonIcon;
begin
  if FEditor=AValue then Exit;
  FEditor:=AValue;
  for I:=Low(Buttons) to High(Buttons) do
    Buttons[I].Enabled:=(AValue <> nil);
end;

procedure TD2DrakonPallet.SetIcons(AValue: TD2DrakonIcons);
var
  I:TD2DrakonIcon;
  W,H:single;
begin
  if FIcons=AValue then Exit;
  FIcons:=AValue;
  RealignButtons;
end;

procedure TD2DrakonPallet.SetInterval(AValue: single);
begin
  if FInterval=AValue then Exit;
  FInterval:=AValue;
  RealignButtons;
end;

procedure TD2DrakonPallet.SetLines(AValue: byte);
begin
  if FLines=AValue then Exit;
  FLines:=AValue;
  RealignButtons;
end;

procedure TD2DrakonPallet.SetOrientation(AValue: TD2Orientation);
begin
  if FOrientation=AValue then Exit;
  FOrientation:=AValue;
  RealignButtons;
end;

constructor TD2DrakonPallet.Create(AOwner: TComponent);
begin
  inherited;
  FIcons:= d2DrakonIconsAll;
  FOrientation:=d2Vertical;
  FMinBtnSize:=d2Point(32,32);
  FInterval:=2.0;
  FLines:=2;
  InitButtons;
  Width:=32*2;
  Height:=32*13;
  ShowHint:=True;
end;

destructor TD2DrakonPallet.Destroy;
begin
  inherited;
end;

//==================== TD2DrakonNode =======================================

//Задать связанный узел ниже
procedure TD2DrakonNode.SetNodeDown(AValue: TD2DrakonNode);
begin
  if FNodeDown=AValue then Exit;
  FNodeDown:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

procedure TD2DrakonNode.FontChanged(Sender: TObject);
begin
  FUpdateEffects:=true;
  Realign;
  Repaint;
end;

procedure TD2DrakonNode.FillChanged(Sender: TObject);
begin
  FUpdateEffects:=true;
  Realign;
  Repaint;
end;

procedure TD2DrakonNode.SetFill(AValue: TD2Brush);
begin
  FFill.Assign(AValue);
  if (FShape<>nil) and (FNodeType<>drComment)
    then FShape.FFill.Assign(AValue);
  if FShape2<>nil
    then FShape2.FFill.Assign(AValue);
  Repaint;
end;

procedure TD2DrakonNode.StrokeChanged(Sender: TObject);
begin
  FUpdateEffects:=true;
  Realign;
  Repaint;
end;

//Задать связанный узел слева
procedure TD2DrakonNode.SetNodeLeft(AValue: TD2DrakonNode);
begin
  if FNodeLeft=AValue then Exit;
  FNodeLeft:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать связанный узел справа
procedure TD2DrakonNode.SetNodeRight(AValue: TD2DrakonNode);
begin
  if FNodeRight=AValue then Exit;
  FNodeRight:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать связанный узел выше
procedure TD2DrakonNode.SetNodeUp(AValue: TD2DrakonNode);
begin
  if FNodeUp=AValue then Exit;
  FNodeUp:=AValue;
  if FEditor <> nil then
    FEditor.Realign;
end;

//Задать тип узела
procedure TD2DrakonNode.SetNodeType(AValue: TD2DrakonNodeType);
var
  t,t2: String;
  fi,fi2: TD2Brush;
  fo,fo2: TD2Font;

begin
  if FNodeType=AValue then Exit;
  FNodeType:=AValue;
  FTextShape:=nil;
  FTextShape2:=nil;
  FShape:=nil;
  FShape2:=nil;
  FNum:=nil;
  case  AValue of     //создаем новый контур
    drAction:    //+Действие
      Resource:='DrakonActionStyle';
    drAddress:   //+Конец ветки-ссылка на другую ветку
      Resource:='DrakonAddressStyle';
    drBeginend:  //+Название алгоритма
      Resource:='DrakonBeginendStyle';
    drBranch:    //+Начало ветки
      Resource:='DrakonBranchStyle';
    drCase:      //+Вариант для множественного выбора
      Resource:='DrakonCaseStyle';
    drComment:   //+Комментарий
      Resource:='DrakonCommentStyle';
    drCommentL:  //+Комментарий слева
      Resource:='DrakonCommentLStyle';
    drCommentR:  //+Комментарий справка ;
      Resource:='DrakonCommentRStyle';
    drDuration:  //+Время, длительность
      Resource:='DrakonDurationStyle';
    drEnd:       //+Конец алгоритма
      Resource:='DrakonEndStyle';
    drInput:     //+Ввод
      Resource:='DrakonInputStyle';
    drInsertion: //+Вставка
      Resource:='DrakonInsertionStyle';
    drLoopBegin: //+Начало цикла
      Resource:='DrakonLoopBeginStyle';
    drLoopEnd:   //+Конец цикла
      Resource:='DrakonLoopEndStyle';
    drOutput:    //+Вывод
      Resource:='DrakonOutputStyle';
    drParams:    //+Вхдные параметры
      Resource:='DrakonParamsStyle';
    drPause:     //+Пауза
      Resource:='DrakonPauseStyle';
    drProcess:   //+Парралельный процесс
      Resource:='DrakonProcessStyle';
    drQuestion:  //+Вопрос
      Resource:='DrakonQuestionStyle';
    drSelect:    //+Множественный выбор из нескольких значений
      Resource:='DrakonSelectStyle';
    drShelf:     //+Полка
      Resource:='DrakonShelfStyle';
    drSInput:    //+Простой ввод
      Resource:='DrakonSInputStyle';
    drSOutput:   //+Простой вывод
      Resource:='DrakonSOutputStyle';
    drTimer:     //+Таймер
      Resource:='DrakonTimerStyle';
    drJunction: //точка соединения линий
      Resource:='DrakonJunctionStyle';
    drArrowLeft: //Стрелка влево
      Resource:='DrakonArrowLStyle';
    drArrowRight://Стрелка вправо
      Resource:='DrakonArrowRStyle';
    drArrowDown: //Стрелка вниз
      Resource:='DrakonArrowDStyle';
    else
         //drNone тип не определен
      Resource:='DrakonNodeStyle';
  end;
end;

procedure TD2DrakonNode.SetStroke(AValue: TD2Brush);
begin
  FStroke.Assign(AValue);
  if FShape<>nil
    then FShape.FStroke.Assign(AValue);
  if FShape2<>nil
    then FShape2.FStroke.Assign(AValue);
  Repaint;
end;

procedure TD2DrakonNode.SetStrokeThickness(AValue: single);
begin
  if FStrokeThickness=AValue then Exit;
  FStrokeThickness:=AValue;
  if FShape<>nil
    then FShape.FStrokeThickness:=AValue;
  if FShape2<>nil
    then FShape2.FStrokeThickness:=AValue;
  Repaint;
end;

procedure TD2DrakonNode.SetText(AValue: String);
begin
  if FText=AValue then Exit;
  FText:=AValue;
  if FTextShape=nil then exit;
  FTextShape.Text:=AValue;
  Realign;
end;

procedure TD2DrakonNode.SetText2(AValue: String);
begin
  if FText2=AValue then Exit;
  FText2:=AValue;
  if FTextShape2=nil then exit;
  FTextShape2.Text:=AValue;
  Realign;
end;

procedure TD2DrakonNode.SetTextFill(AValue: TD2Brush);
begin
  FTextFill.Assign(AValue);
  if FTextShape<>nil
    then FTextShape.Fill.Assign(AValue);
  if FTextShape2<>nil
    then FTextShape2.Fill.Assign(AValue);
  //Realign;
end;

procedure TD2DrakonNode.SetTextFont(AValue: TD2Font);
begin
  FTextFont.Assign(AValue);
  if FTextShape<>nil
    then FTextShape.FFont.Assign(AValue);
  if FTextShape2<>nil
    then FTextShape2.FFont.Assign(AValue);
  //Realign;
end;

procedure TD2DrakonNode.ApplyStyle;
var S:TD2Object;
begin
  inherited;
  S:=FindResource('shape');
  if (S<>nil) and (S is TD2Shape)
    then FShape:=TD2Shape(S)
    else if (S=nil) and (FResourceLink is TD2Shape)
           then FShape:=TD2Shape(FResourceLink);
  S:=FindResource('shape2');
  if (S<>nil) and (S is TD2Shape)
    then FShape2:=TD2Shape(S);
  S:=FindResource('text');
  if (S<>nil) and (S is TD2Text)
    then FTextShape:=TD2Text(S);
  S:=FindResource('text2');
  if (S<>nil) and (S is TD2Text)
    then FTextShape2:=TD2Text(S);
  S:=FindResource('number');
  if (S<>nil) and (S is TD2Text)
    then  begin
            FNum:=TD2Text(S);
            FNum.Visible:=false;
          end;
  Realign;
end;

constructor TD2DrakonNode.Create(AOwner: TComponent);
begin
  inherited;
  FNodeIndex:=-1;
  FNodeType:=drNone;
  FShape:= nil;
  FShape2:= nil;
  FTextShape:= nil;
  FTextShape2:= nil;
  FNodeDown:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  FNodeUp:= nil;
  FText:='';
  FText2:='';
  FTextFont:=TD2Font.Create;
  FTextFont.OnChanged:=FontChanged;

  FTextFill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FTextFill.SolidColor:=$FF000000;
  FTextFill.OnChanged:=FillChanged;

  FFill:=TD2Brush.Create(d2BrushSolid,$FFE0E0E0);
  FFill.SolidColor:=$FFE0E0E0;
  FFill.OnChanged:=FillChanged;

  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FStroke.SolidColor:=$FF000000;
  FStroke.OnChanged:=StrokeChanged;

  FStrokeThickness:=1;
end;

destructor TD2DrakonNode.Destroy;
begin
  FNodeDown:= nil;
  FNodeUp:= nil;
  FNodeLeft:= nil;
  FNodeRight:= nil;
  inherited;
end;

procedure TD2DrakonNode.Realign;
var
  h,h1: single;

  function AdjustHeight(AText: TD2Text): single;
  var
    R: TD2Rect;
  begin
    if (AText <> nil) and (AText.Canvas <> nil) and (AText.FText <> '') then
      with AText do
      begin
        Canvas.Font.Assign(FFont);
        R:=d2Rect(0,0,Width,1000);
        Canvas.MeasureText(R,R,FText,true,d2TextAlignNear,d2TextAlignNear);
        Result:=R.Bottom;
      end
    else Result:=0;
  end;

begin
  if csDestroying in ComponentState then Exit;
  if FDisableDefaultAlign then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;

  if Width < 40
    then Width:= 40 ;
  if FTextShape2 <> nil
    then begin
           FTextShape2.Text:=FText2;
           FTextShape2.FFont.Assign(FTextFont);
           FTextShape2.FFill.Assign(FTextFill);
           h1:=AdjustHeight(FTextShape2) + FTextShape2.FPadding.FTop + FTextShape2.FPadding.FBottom;
           if h1<15 then h1:=15;
         end
    else h1:=0;
  if FShape2 <> nil
    then begin
           h1:= h1 + FShape2.FMargins.FTop + FShape2.FMargins.FBottom;
           h:= h1 + FShape2.FPadding.FTop + FShape2.FPadding.FBottom;
           FShape2.FFill.Assign(FFill);
           FShape2.FStroke.Assign(FStroke);
           FShape2.FStrokeThickness:=FStrokeThickness;
           FShape2.Height:=h1;
         end
    else h:=0;
  if FTextShape <> nil
    then begin
           FTextShape.Text:=FText;
           FTextShape.FFont.Assign(FTextFont);
           FTextShape.FFill.Assign(FTextFill);
           h1:=AdjustHeight(FTextShape) + FTextShape.FPadding.FTop + FTextShape.FPadding.FBottom;
           if h1<15 then h1:=15;
         end
    else h1:=0;
  if FShape <> nil then
  begin
    FShape.FStroke.Assign(FStroke);
    FShape.FStrokeThickness:=FStrokeThickness;
    if FNodeType=drComment
      then h:= h + h1 + FShape.FMargins.FTop + FShape.FMargins.FBottom
      else begin
             FShape.FFill.Assign(FFill);
             h1:= h1 + FShape.FMargins.FTop + FShape.FMargins.FBottom;
             h:= h + h1 + FShape.FPadding.FTop + FShape.FPadding.FBottom;
             FShape.Height:=h1;
           end;
  end;
  if h<15 then h:=15;
  if h>1000 then h:=1000;
  Height:=h;
  inherited;
end;

//==================== TD2CustomDrakonEditor =======================================
 
function TD2CustomDrakonEditor.LinkNode(AIndex,ALeft,AUp,ARight,ADown: integer): boolean;
  //Перемещает и связывает узел с индексом AIndex в списке узлов FNodes с узлами
  //с индексами ALeft,AUp,ARight,ADown, соотвественно слева, сверху, справа и снизу.
var I,L,U,R,D: TD2DrakonNode;
begin
  Result:=false;
  //если списка узлов FNodes нет или индексы перемещаемого узла не находятся в списке, то выходим
  if (FNodes=nil) or (AIndex < 0) or (AIndex >= FNodes.Count) then exit;

  I:=TD2DrakonNode(FNodes.Items[AIndex]);
  if (ALeft>=0) and (ALeft < FNodes.Count)
    then L:=TD2DrakonNode(FNodes.Items[ALeft])
    else L:=nil;
  if (AUp >= 0) and (AUp < FNodes.Count)
    then U:=TD2DrakonNode(FNodes.Items[AUp])
    else U:=nil;
  if (ARight >= 0) and (ARight < FNodes.Count)
    then R:=TD2DrakonNode(FNodes.Items[ARight])
    else R:=nil;
  if (ADown >= 0) and (ADown < FNodes.Count)
    then D:=TD2DrakonNode(FNodes.Items[ADown])
    else D:=nil;

  I.FNodeLeft:=L;
  I.FNodeUp:=U;
  I.FNodeRight:=R;
  I.FNodeDown:=D;

  if L <> nil then L.FNodeRight:=I;
  if U <> nil then U.FNodeDown:=I;
  if R <> nil then R.FNodeLeft:=I;
  if D <> nil then D.FNodeUp:=I;

  Result:=false;
end;

procedure TD2CustomDrakonEditor.NodeClick(Sender: TObject);
begin
  SelectNode(TD2DrakonNode(Sender));
end;

procedure TD2CustomDrakonEditor.SelectNode(ANode: TD2DrakonNode);
begin
  if FSelectNode=ANode then exit;
  BeginUpdate;
  if ANode.FNodeType in [drNone, drJunction, drArrowDown,drArrowLeft,drArrowRight]
    then  begin //обрабатываем узлы, линии, стрелки

          end
    else  begin //обрабатываем иконы
           if FSelectNode<>nil
             then  begin
                     FSelectNode.Align:=vaNone;
                     FSelectNode.SetBounds(FSelection.Position.X,FSelection.Position.Y,FSelection.Width,FSelection.Height);
                     FSelectNode.Parent:=Self;
                     FSelectNode.HitTest:=True;
                   end;
           if ANode=nil
              then FSelection.Visible:=False
              else  begin
                      FSelection.Visible:=True;
                      FSelection.SetBounds(ANode.Position.X,ANode.Position.Y,ANode.Width,ANode.Height);
                      ANode.Parent:=FSelection;
                      ANode.HitTest:=False;
                      ANode.Align:=vaClient;
                  end;
          end;
  FSelectNode:=ANode;
  EndUpdate;
end;

procedure TD2CustomDrakonEditor.SelectionTrack(Sender: TObject);
begin
  FSelection.Height:=FSelectNode.Height;
  if FSelection.Width<FSelectNode.Width
     then FSelection.Width:=FSelectNode.Width;
end;

function TD2CustomDrakonEditor.CreateNode(ABounds: TD2Rect; AType: TD2DrakonNodeType;
           ALeft,AUp,ARight,ADown: integer; AText, AText2: String): integer;
//Добавляет новый узел в поле редактора. Возвращает индекс в списке FNodes
       //ABounds - Left - X, Top - Y, Right - Width, Bottom - Height; AType - тип узла;
       //ALeft,AUp,ARight,ADown - индексы в списке FNodes узлов, связанных с создаваемым узлом, соотвественно слева, сверху, справа и снизу
       //AText, AText2 - текст 1 и 2 на иконе соответственно
var N: TD2DrakonNode;
begin
  N:=TD2DrakonNode.Create(Self);
  N.Parent:=Self;
  N.NodeType:=AType;
  N.Text:=AText;
  N.Text2:=AText2;
  //N.BoundsRect:=ABounds;
  N.Position.Point:=ABounds.TopLeft;
  N.Width:=ABounds.Right;
  Result:=FNodes.Add(N);
  N.FNodeIndex:=Result;
  N.OnClick:=NodeClick;

  LinkNode(Result, ALeft,AUp,ARight,ADown);
end;

procedure TD2CustomDrakonEditor.CreateSilhouette;
var N: TD2DrakonNode;
begin
  BeginUpdate;

  ClearNodes;
  //0 икона начало
  CreateNode(d2Rect(50,20,130,17.5), drBeginend, -1,-1,-1,-1,'Новый алгоритм', '');
  //1 стрелка возврата
  CreateNode(d2Rect(50,60,50,15), drArrowRight, -1, 0,-1,-1,'', '');
  //2 начало ветки 1
  CreateNode(d2Rect(50,90,60,32.5), drBranch,   -1, 1,-1,-1,'Ветка 1', '');
  //3 конец ветки 1
  CreateNode(d2Rect(50,150,60,32.5), drAddress,  -1, 2,-1,-1,'Ветка 2', '');
  //4 узел под веткой 1
  CreateNode(d2Rect(50,210,40,15), drJunction,   -1, 3,-1,-1,'', '');
  //5 узел слева под веткой 1
  CreateNode(d2Rect(20,210,40,15), drJunction,     -1,-1, 4,-1,'', '');
  //6 узел слева над веткой 1
  CreateNode(d2Rect(20,60,40,15), drJunction,     -1,-1, 1, 5,'', '');
  //7 узел над веткой 2
  CreateNode(d2Rect(200,60,40,15), drJunction,  1,-1,-1,-1,'', '');
  //8 начало ветки 2
  CreateNode(d2Rect(200,90,60,32.5), drBranch, -1, 7,-1,-1,'Ветка 2', '');
  //9 икона конец
  CreateNode(d2Rect(200,150,50,17.5), drEnd,    -1, 8,-1,-1,'Конец', '');

  EndUpdate;
end;

procedure TD2CustomDrakonEditor.ClearNodes;
var i: integer;
begin
  if FNodes <> nil then
    for i:=0 to FNodes.Count-1 do
      TObject(FNodes[i]).Free;
  FNodes.Clear;
end;

constructor TD2CustomDrakonEditor.Create(AOwner: TComponent);
begin
  inherited;
  FNodes:=TList.Create;
  Resource:='FramedScrollBoxStyle';
  FSelection:=TD2Selection.Create(Self);
  FSelection.Parent:=Self;
  FSelection.Visible:=False;
  FSelection.DesignHide:=True;
  FSelection.SizingVert:=false;
  FSelection.OnTrack:=SelectionTrack;
  //FSelection.OnMouseMove:=SelectionMouseMove;
end;

destructor TD2CustomDrakonEditor.Destroy;
var i: integer;
begin
  ClearNodes;
  FNodes.Free;
  FSelection.Free;
  inherited;
end;

//выполнить команду
function TD2CustomDrakonEditor.SetCommand(ACommand: TD2DrakonIcon): boolean;
begin
  try
    Case ACommand of
      driSilhouette:   //силуэт/примитив
        begin
          CreateSilhouette;
        end;
      driBranch:       //Начало ветки
        begin
        end;
      driAction:       //Действие
        begin
        end;
      driQuestion:     //Вопрос
        begin
        end;
      driSelect:       //Множественный выбор из нескольких значений
        begin
        end;
      driCase:         //Вариант для множественного выбора
        begin
        end;
      driInsertion:    //Вставка
        begin
        end;
      driParams:       //Вхдные параметры
        begin
        end;
      driLoop:         //Цикл
        begin
        end;
      driComment:      //Комментарий
        begin
        end;
      driCommentL:     //Комментарий слева
        begin
        end;
      driCommentR:     //Комментарий справа
        begin
        end;
      driParallel:     //одновременная работа
        begin
        end;
      driMoreParallel: //добавить одновременное исполнение
        begin
        end;
      driInput:        //Ввод
        begin
        end;
      driOutput:       //Вывод
        begin
        end;
      driSInput:       //Простой ввод
        begin
        end;
      driSOutput:      //Простой вывод
        begin
        end;
      driProcess:      //Парралельный процесс
        begin
        end;
      driShelf:        //Полка
        begin
        end;
      driTimer:        //Таймер
        begin
        end;
      driDuration:     //Время, длительность
        begin
        end;
      driPause:        //Пауза
        begin
        end;
      driZoom:         //масштаб
        begin
        end;
    end;
  finally
    Result:=true;
  end;
end;





















  
