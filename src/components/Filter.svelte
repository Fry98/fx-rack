<script lang="ts">
  import Device from "./Device.svelte";
  import Knob from "./Knob.svelte";

  export let type: boolean;
  export let cutoff: number;
  export let slope: number;
</script>

<Device name="LP/HP Filter" on:remove>
  <div class='filter'>
    <div class='type'>
      <div class='option' on:click={() => type = false}>
        <div class='radio' class:uncheck={type}></div>
        <div class='label'>Low-pass</div>
      </div>
      <div class='option' on:click={() => type = true}>
        <div class='radio' class:uncheck={!type}></div>
        <div class='label'>High-pass</div>
      </div>
    </div>
    <div class='knob-center'>
      <Knob
        name='Cutoff'
        lowerBound={0}
        upperBound={1}
        bind:value={cutoff}
      />
    </div>
    <Knob
      name='Slope'
      lowerBound={1}
      upperBound={100}
      bind:value={slope}
    />
  </div>
</Device>

<style lang="scss">
  .filter {
    display: flex;
    align-items: center;
    justify-content: center;
    margin-bottom: 15px;
  }

  .radio {
    width: 17px;
    height: 17px;
    border-radius: 50%;
    background: rgb(50, 50, 53);
    margin-right: 4px;
    position: relative;
    overflow: hidden;

    &::after {
      content: '';
      background: rgb(55, 179, 243);
      position: absolute;
      border-radius: 50%;
      top: 0px;
      left: 0px;
      width: 100%;
      height: 100%;
      transform: scale(.5);
    }
  }

  .uncheck::after {
    opacity: 0;
  }

  .option {
    display: flex;
    align-items: center;
    margin: 5px 0px;
    cursor: pointer;
  }

  .knob-center {
    margin-left: 50px;
    margin-right: 20px;
  }
</style>
