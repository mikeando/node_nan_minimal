{
  "targets": [{
    "target_name": "node_nan_minimal",
    "include_dirs" : [
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "node_nan_minimal.cc"
    ],
    "cflags": [
      "-std=c++11",
    ]
  }]
}
