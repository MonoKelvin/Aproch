#ifndef APROCH_H
#define APROCH_H

#define APROCH_PLATFORM_WINDOWS

#ifdef APROCH_PLATFORM_WINDOWS
#  define APROCH_EXPORT __declspec(dllexport)
#  define APROCH_IMPORT __declspec(dllimport)
#  define APROCH_PRIVATE
#else
#  define APROCH_EXPORT
#  define APROCH_IMPORT
#  define APROCH_LOCAL
#endif

//#ifdef __cplusplus
//#  define APROCH_DEMANGLED extern "C"
//#else
//#  define APROCH_DEMANGLED
//#endif

#define APROCH_NAMESPACE_BEGIN namespace Aproch {
#define APROCH_NAMESPACE_END }

#define AAbs(x) ((x)<0?-(x):(x))
#define AMax(a, b) ((a)>(b)?(a):(b))
#define AMin(a, b) ((a)<(b)?(a):(b))

#define JSON_VALUE_EXISTS(_JsonValueRef_)              \
    (_JsonValueRef_.type() != QJsonValue::Undefined && \
     _JsonValueRef_.type() != QJsonValue::Null)

#define READ_COLOR_FROM_JSON(_JsonObject_, _VarName_)                     \
    do{                                                                   \
        auto valueRef = _JsonObject_[#_VarName_];                         \
        if (JSON_VALUE_EXISTS(valueRef) && valueRef.isString())           \
        {                                                                 \
            _VarName_ = QColor(valueRef.toString());                      \
        }                                                                 \
        else                                                              \
        {                                                                 \
            qWarning() << "Error json type for parameter:" << #_VarName_; \
        }                                                                 \
    }while(0)
#define READ_FLOAT_FROM_JSON(_JsonObject_, _VarName_)                     \
    do{                                                                   \
        auto valueRef = _JsonObject_[#_VarName_];                         \
        if (JSON_VALUE_EXISTS(valueRef) && valueRef.isDouble())           \
        {                                                                 \
            _VarName_ = float(valueRef.toDouble());                       \
        }                                                                 \
        else                                                              \
        {                                                                 \
            qWarning() << "Error json type for parameter:" << #_VarName_; \
        }                                                                 \
    }while(0)
#define READ_VALUE_FROM_JSON(_JsonObject_, _VarName_, _Type_)                \
    do{                                                                      \
        auto valueRef = _JsonObject_[#_VarName_];                            \
        if (JSON_VALUE_EXISTS(valueRef) && valueRef.is##_Type_())            \
        {                                                                    \
            _VarName_ = valueRef.to##_Type_();                               \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            qWarning() << "Error or undefined json type for " << #_VarName_; \
        }                                                                    \
    }while(0)

#define INVALID_PORT_INDEX 0

#define NODE_BORDER_RADIUS 3.0

#endif // APROCH_H
