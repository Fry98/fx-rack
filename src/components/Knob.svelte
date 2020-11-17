<script lang="ts">
  import { getContext, onDestroy } from "svelte";
  import type { Writable } from "svelte/store";

  const precision: Writable<boolean> = getContext('precision');

  export let lowerBound: number;
  export let upperBound: number;
  export let value: number;
  export let name: string;
  let drag = false;

  $: knobRotation = () => {
    const range = upperBound - lowerBound;
    const adjValue = value - lowerBound;
    const ratio = adjValue / range;
    const degree = (270 * ratio) - 135;
    const degStr = `${Math.round(degree)}deg`;
    return degStr;
  };

  const mousedown = () => drag = true;
  const mouseup = () => drag = false;
  const mousemove = (e: MouseEvent) => {
    if (!drag) return;
    const range = upperBound - lowerBound;
    const unit = range / 270 * ($precision ? 0.3 : 1);
    const change = -e.movementY * unit;
    const newVal = value + change;
    value = Math.min(Math.max(newVal, lowerBound), upperBound);
  };

  document.addEventListener('mousemove', mousemove);
  document.addEventListener('mouseup', mouseup);

  onDestroy(() => {
    document.removeEventListener('mousemove', mousemove);
    document.removeEventListener('mouseup', mouseup);
  });
</script>

<div
  class='knob-wrap'
  on:mousedown={mousedown}
  style="--knob-rot: {knobRotation()}"
>
  <div class='knob' />
  <div class='knob-desc'>{name}</div>
</div>

<style lang='scss'>
  .knob-wrap {
    display: inline-block;
  }

  .knob-desc {
    background: rgba(0, 0, 0, 0.24);
    padding: 2px 0px;
    margin: 0px 7px;
    font-size: .8em;
    color: white;
  }

  .knob {
    width: 85px;
    height: 85px;
    background-image: url('../assets/knob_main.png');
    background-size: contain;
    background-position: center center;
    position: relative;
    cursor: pointer;
    margin-bottom: -7px;

    &::after {
      content: '';
      position: absolute;
      top: 0px;
      left: 0px;
      width: 100%;
      height: 100%;
      background-image: url('../assets/knob_pointer.png');
      background-size: contain;
      background-position: center center;
      transform: rotate(var(--knob-rot));
    }
  }
</style>
