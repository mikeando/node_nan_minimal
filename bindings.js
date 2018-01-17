var node_nan_minimal

if (process.env.DEBUG) {
  node_nan_minimal=require('./build/Debug/node_nan_minimal.node')
} else {
  node_nan_minimal=require('./build/Release/node_nan_minimal.node')
}

module.exports = node_nan_minimal
