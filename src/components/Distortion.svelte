<script lang="ts">
  import Device from "./Device.svelte";
  import Knob from "./Knob.svelte";

  export let gain: number;
  export let threshold: number;
</script>

<Device name="Distortion" on:remove>
  <div class="distortion">
    <Knob
      name="Gain"
      lowerBound={0.1}
      upperBound={4.0}
      bind:value={gain}
    />
    <Knob
      name="Threshold"
      lowerBound={100}
      upperBound={32767}
      bind:value={threshold}
    />
  </div>
</Device>

<style lang="scss">
  .distortion {
    display: flex;
    align-items: center;
    justify-content: space-evenly;
    margin-bottom: 15px;
  }
</style>
