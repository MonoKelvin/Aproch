import { AInterface } from '../Aprochh';

export interface ITypeConverter {
    canConvert(itf1: AInterface, itf2: AInterface): boolean;
    convertTo(dm1);
}

export class ABaseTypeConverter implements ITypeConverter {
    constructor() {
    }

    canConvert(itf1: AInterface, itf2: AInterface): boolean {
        throw new Error('Method not implemented.');
    }
    convertTo(dm1: any) {
        throw new Error('Method not implemented.');
    }
}
