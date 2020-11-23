export interface AudioMeta {
  length: number;
  rate: number;
}

export enum DeviceType {
  FILTER = 0,
  REVERB = 1,
  DISTORTION = 2
}

export interface Device {
  id: number;
  type: DeviceType;
  [K: string]: any;
}
