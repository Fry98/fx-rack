export interface AudioMeta {
  length: number;
  rate: number;
}

export enum DeviceType {
  FILTER,
  REVERB,
  DISTORTION
}

export interface Device {
  id: number;
  type: DeviceType;
  [K: string]: any;
}
