import { IndicatorBase } from './IndicatorBase';

export interface IndicatorParams {
    period: number;
    appliedPrice: number;
    shift: number;
}

export class IndicatorData extends IndicatorBase {
    public constructor(params: IndicatorParams) {
        super();
    }
}

