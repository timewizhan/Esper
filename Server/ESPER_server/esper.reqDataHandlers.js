function str2obj (data) {
    var dataObj = JSON.parse(data);
    return dataObj;
}

function obj2str (data) {
    var dataStr = JSON.stringify(data);
    return dataStr;
}

exports.str2obj = str2obj;
exports.obj2str = obj2str;

