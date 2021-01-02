<script lang="ts">
  import Device from "./Device.svelte";
  import Knob from "./Knob.svelte";

  export let delay: number;
  export let decayFactor: number;
  export let mix: number;
</script>

<Device name="Reverb" on:remove>
  <div class="reverb">
    <Knob
      name="Delay"
      lowerBound={50}
      upperBound={1000}
      bind:value={delay}
    />
    <Knob
      name="Decay"
      lowerBound={0.1}
      upperBound={1.0}
      bind:value={decayFactor}
    />
    <Knob
      name="Dry / Wet"
      lowerBound={0}
      upperBound={100}
      bind:value={mix}
    />
  </div>
</Device>

<style lang="scss">
  .reverb {
    display: flex;
    align-items: center;
    justify-content: space-evenly;
    margin-bottom: 15px;
  }
</style>
