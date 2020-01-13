export class ITypeConverter {
    constructor() {}

    static CanConvert(itf1, itf2) {
        return false;
    }

    static ConvertTo(dm1) {
        // return IDataModel();
    }
}

export class ABaseTypeConverter extends ITypeConverter {
    constructor() {
        super();
    }

    static CanConvert(itf1, itf2) {
        return true;
    }
}
